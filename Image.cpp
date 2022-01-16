#include "Image.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <cmath>

#define M_PI 3.14159265358979323846

// funkcje pomocnicze

image_pixel square_pixel(const image_pixel& pixel)
{
    image_pixel temp(pixel.size_);
    for(int i = 0; i < pixel.size_; i++)
    {
        uint16_t val = (uint16_t)(sqrt(pixel.pixel_[i]));
        temp.pixel_[i] = val;
    }
    return temp;
}

// metody klasy

Image::Image(const std::string& filename)
{
    std::ifstream input_stream(filename, std::ios::in);
    if (input_stream.is_open()) // gdy plik zostal otwarty wykonujemy ponizsze operacje
    {
        ReadHeader(input_stream); // czytam naglowek pliku
        
        pixels.reserve(width * height); // rezerwujemy odpowiednia ilosc pamieci w wektorze

        ReadPixels(input_stream); // wczytujemy piksele z obrazu
    }
    else
    {
        std::cout << "Error. Unable to open " << filename << std::endl;
    }

    input_stream.close(); 
}

Image::~Image()
{
    for (int i = 0; i < pixels.size(); i++)
    {
        delete pixels[i];
    }
}

void Image::ReadHeader(std::ifstream& input_stream)
{
    std::string line;
    std::string rest;
    int step;
    std::string word;

    step = 0;

    while (1)
    {
        std::getline(input_stream, line); 

        if (input_stream.eof()) 
        {
            std::cout << "error" << std::endl;
            exit(1);
        }

        if (line[0] == '#') // jesli # to komentarz, wiec pomijamy linie do konca
        {
            continue;
        }

        if (step == 0)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
            
            line = rest;

            // sprawdzamy naglowek pliku, powinien skladac sie z dwoch znakow - P1, P2 albo P3
            if ((word[0] != 'P' && word[0] != 'p') || (word[1] != '1' && word[1] != '2' && word[1] != '3'))
            {
                std::cout << "error" << std::endl;
                exit(1);
            }
            if(word[1] == '1')
            {
                magic_number = "P1";
                channels = 1;
            }
            else if(word[1] == '2')
            {
                magic_number = "P2";
                channels = 1;
            }
            else if(word[1] == '3')
            {
                magic_number = "P3";
                channels = 3;
            }

            step = 1;
        }

        // pobieramy kolejny parametr - szerokosc obrazu z naglowka
        if (step == 1)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
            
            width = atoi(word.c_str());
            line = rest;
            step = 2;
        }

        // pobieramy kolejny parametr - wysokosc obrazu z naglowka
        if (step == 2)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
           
            height = atoi(word.c_str());
            line = rest;
            break;
        }

        // pobieramy kolejny parametr - skale szarosci z naglowka
        if (step == 3)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
            
            max_pixel_value = atoi(word.c_str());
            break;
        }

    }

    return;
}

void Image::ReadPixels(std::ifstream& input_stream)
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            image_pixel* pixel = new image_pixel(channels);
            input_stream >> *pixel;
            pixels.push_back(pixel);
        }
    }

    return;
}

void Image::SaveImage(const std::string& filename)
{

    std::ofstream output_stream(filename.c_str(), std::ios::out);

    if (output_stream.is_open())
    {
        output_stream << magic_number << "\n";
        output_stream << width;
        output_stream << " ";
        output_stream << height << "\n";

        if (channels == 2 || channels == 3) output_stream << max_pixel_value << "\n";

        for (unsigned int j = 0; j < height; j++)
        {
            for (unsigned int i = 0; i < width; ++i)
            {
                output_stream << *pixels[j*width + i];
            }
            output_stream << "\n";
        }
    }
    else
    {
        std::cout << "Error. Unable to open " << filename << std::endl;
    }
    output_stream.close();
}

void Image::ResizeImage(int new_width, int new_height)
{
    // tworzymy tymczasowy wektor o rozmiarze nowego obrazu
    std::vector<image_pixel*>temp(new_width * new_height);

    // szukamy wspolczynnika skalowania dla osi poziomej i pionowej
    float x_scale = width / (float)new_width;
    float y_scale = height / (float)new_height;

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            image_pixel* output_color = new image_pixel(channels);
            // szukamy jaki piksel w oryginalnym obrazie
            // bedzie odpowiadal pikselowi w nowym wiekszym obrazie
            // zgodnie z algorytmem nearest neighbor interpolation
            int xp = floor(x * x_scale);
            int yp = floor(y * y_scale);

            // umieszczamy piksel w przetworzonym obrazie
            *(output_color) = *(pixels[yp * width + xp]);
            temp[y * new_width + x] = output_color;
        }
    }

    // podmieniamy rozmiaru obrazu
    width = new_width;
    height = new_height;

    // podmieniamy piksele obrazu na nowe
    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    } 
}

void Image::RotateImage(float angle)
{
    // tworzymy tymczasowy wektor ktory bedzie przechowywac przetworzony obraz
    std::vector<image_pixel*>temp(width * height); 
    
    // konwertujemy stopnie na radiany,
    // funkcja takie jak sin czy cos ze standardowej biblioteki
    // jako parametr przyjmuja liczbe w radianach
    float _angle = (-angle * M_PI) / 180;
    
    // znajdujemy srodkowy piksel,
    // bedzie to pozycja wokol ktorej bedziemy wykonywac obrot 
    float xs = width / 2.0;
    float ys = height / 2.0;

     for (int y = 0; y < height; y++)
        {
         for (int x = 0; x < width; x++)
         {
            image_pixel* output_color = new image_pixel(channels);

            // wyznaczamy nowa pozycje piksela zgodnie z wzorem zawartym w instrukcji pdf
            int xp = int(cos(_angle) * (x - xs) - sin(_angle) * (y - ys) + xs);
            int yp = int(sin(_angle) * (x - xs) + cos(_angle) * (y - ys) + ys);
        
            // sprawdzamy ktore z pikseli powinny zostac umieszczone w obrazie wynikowym
            if ((0 <= xp && xp < width) && ((0 <= yp && yp < height)))
            {
                *(output_color) = *(pixels[yp * width + xp]);
                temp[y * width + x] = output_color;
            }
         }
     } 
    // podmieniamy piksele obrazu na nowe
    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    } 
}

void Image::BinaryImage(const short& treshold)
{
    if (treshold > max_pixel_value && treshold < 0)
    {
        std::cerr << "Bledna wartosc graniczna flagi -ib, wartosc musi byc ustawiona na od 0 do " << max_pixel_value << '\n';
        exit(1);
    }

    else if (magic_number == "P1")
    {
        std::cerr << "Bledny rodzaj pliku, nalezy uzyc pliku typu .pgm albo .ppm";
        exit(1);
    }
    else
    {
        for (int i = 0; i < pixels.size(); i++)
        {
            if (*(pixels[i]) > treshold) pixels[i]->set_pixel(treshold);
            else pixels[i]->set_pixel(0);
        }
        // porownjemy piksel z progiem i ustawiamy odpowiednia wartosc dla niego
        // std::for_each(pixels.begin(), pixels.end(), [&](image_pixel& x)
        // {
        //     if (x > treshold) x.set_pixel(treshold);
        //     else x.set_pixel(0);
        // });
    }
}

void Image::ApplyBlur(const std::string& method)
{ 
    std::vector<int> testY = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
    std::vector<int> testX = {-1,  0,  1, -1, 0, 1, -1, 0, 1};

    // tworzymy tymczasowy wektor na przetworzone piksele
    std::vector<image_pixel*>temp(width * height);

    // tworzymy maske
    const int MASK_HEIGHT_SIZE = 3;
    const int MASK_WIDTH_SIZE = 3;

    std::vector<float>mask(MASK_WIDTH_SIZE * MASK_HEIGHT_SIZE);

    // suma wag, przez którą dzielimy każdy piksel
    float coefficient = 0.0;

    // wartości macierzy maski dobrane w zależności od metody rozmycia
    if (method == "average")
    {
        mask = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
        coefficient = 9.0;
    }

    if (method == "gauss")
    {
        mask = {1.0, 2.0, 1.0, 2.0, 4.0, 2.0, 1.0, 2.0, 1.0};
        coefficient = 16.0;
    }
     
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            image_pixel* output_color = new image_pixel(channels);

            for (uint32_t i = 0; i < mask.size(); i++)
            {
                int target_x = x + testX[i];
                int target_y = y + testY[i];

                if (target_y >= 0 && target_y < height && target_x >= 0 && target_x < width)
                {
                    *output_color = *(pixels[target_y * width + target_x]) * mask[i];
                }

            }

            // dzielimy otrzymany kolor przez współczynnik (sume wartości macierzy maski) czyli usredniamy wynik
            *(output_color) /= coefficient;
             
            // umieszczamy nowy 'rozmyty' piksel w tablicy
            temp[y * width + x] = output_color;
        }
    }

    // podmieniamy piksele obrazu na nowe
    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    }
}

// filtr medianowy
void Image::ReduceNoise()
{
    // tworzymy tymczasowy wektor na przetworzone piksele
    std::vector<image_pixel*>temp(width * height);

    // wektor bedzie przechowywal piksele z okna 3x3
    // ktore pozniej zostana posortowane
    std::vector<image_pixel*> median_filter;

    std::vector<int> testY = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
    std::vector<int> testX = {-1,  0,  1, -1, 0, 1, -1, 0, 1};

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            image_pixel* output_color = new image_pixel(channels);

            for (uint32_t i = 0; i < median_filter.size(); i++)
            {
                int target_x = x + testX[i];
                int target_y = y + testY[i];

                if (target_y >= 0 && target_y < height && target_x >= 0 && target_x < width)
                {
                    *(output_color) = *(pixels[target_y * width + target_x]);
                    median_filter.push_back(output_color);
                }

            }

            // sortujemy piksele rosnaco
            std::sort(median_filter.begin(), median_filter.end(), [&median_filter](image_pixel*& x, image_pixel*& y) { return x < y; });
            // BubbleSort(median_filter);

            // wybieramy srodkowy piksel i umieszczamy go w nowej tablicy z pikselami
            temp[y * width + x] = median_filter[floor(median_filter.size()/2)];

            // czyscimy tablice
            median_filter.clear();
        }
    }

    // podmieniamy piksele obrazu na nowe
    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    }
}

void Image::NegativeImage()
{
    for (int i = 0; i < pixels.size(); i++)
    {
        *(pixels[i]) = max_pixel_value - *(pixels[i]);
    }
    // std::for_each(pixels.begin(), pixels.end(), [&](image_pixel& x) { x = max_pixel_value - x; return x; });
}

void Image::GradientImage(const std::string& method)
{
    std::vector<int> testY;
    std::vector<int> testX;

    // Tworzona jest tablica - maska
    std::vector<float> gx_mask;
    std::vector<float> gy_mask;

    // Rozmiary tablicy - maski
    int MASK_HEIGHT = 0;
    int MASK_WIDTH = 0;

    // Sobel
    if (method == "1")
    {
        MASK_HEIGHT = 3;
        MASK_WIDTH = 3;

        gx_mask = {1, 0, -1, 2, 0, -2,  1,  0, -1};
        gy_mask = {1, 2,  1, 0, 0,  0, -1, -2, -1};

        testY = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
        testX = {-1,  0,  1, -1, 0, 1, -1, 0, 1};
    }

    // Prewitt
    else if (method == "2")
    {
        MASK_HEIGHT = 3;
        MASK_WIDTH = 3;

        gx_mask = {1, 0, -1, 1, 0, -1, 1, 0, -1};
        gy_mask = {1, 1, 1, 0, 0, 0, -1, -1, -1};

        testY = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
        testX = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    }

    // Roberts
    else // (method == "3")
    {
        MASK_HEIGHT = 2;
        MASK_WIDTH = 2;

        gx_mask = {1, 0, 0, -1};
        gy_mask = {0, 1, -1, 0};

        testY = {-1, -1,  0, 0};
        testX = {-1,  0, -1, 0};
    }

    std::vector<image_pixel*>temp(width * height);

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            image_pixel* output_color = new image_pixel(channels);
            image_pixel color_x(channels);
            image_pixel color_y(channels);

            image_pixel temp_color(channels);

            for (uint32_t i = 0; i < gx_mask.size(); i++)
            {
                int target_x = x + testX[i];
                int target_y = y + testY[i];

                if (target_y >= 0 && target_y < height && target_x >= 0 && target_x < width)
                {     
                    color_x += *(pixels[target_y * width + target_x]) * gx_mask[i];
                    color_y += *(pixels[target_y * width + target_x]) * gy_mask[i];
                }
            }

            temp_color = color_y * color_y;
            temp_color += color_x * color_x;
            *output_color = square_pixel(temp_color);

            if (*output_color > max_pixel_value)
                *output_color = max_pixel_value;
            else if (*output_color < 0)
                *output_color  = 0;
             
            temp[y * width + x] = output_color;
        }
    }

    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    }
}

void Image::ErodeImage()
{
    if (channels != 1)
    {
        std::cerr << "Nieprawidlowy format obrazu. Uzyj obrazu binarnego z rozszezeniem .pbm\n";
        exit(1);
    }
    std::vector<image_pixel*> temp(width * height);

    std::vector<int> testY = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
    std::vector<int> testX = {-1,  0,  1, -1, 0, 1, -1, 0, 1};

    std::vector<int> mask = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image_pixel* output_color = new image_pixel(channels);

            for (uint32_t i = 0; i < mask.size(); i++)
            {
                int target_x = x + testX[i];
                int target_y = y + testY[i];

                if (target_y >= 0 && target_y < height && target_x >= 0 && target_x < width)
                {     
                    *(output_color) &= *(pixels[target_y * width + target_x]) & mask[i];
                }
            }
            temp[y * width + x] = output_color;
        }
    }

    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    }
}

void Image::DilateImage()
{
    if (channels != 1)
    {
        std::cerr << "Nieprawidlowy format obrazu. Uzyj obrazu binarnego z rozszezeniem .pbm\n";
        exit(1);
    }
    std::vector<image_pixel*> temp(width * height);

    std::vector<int> testY = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
    std::vector<int> testX = {-1,  0,  1, -1, 0, 1, -1, 0, 1};

    std::vector<int> mask = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image_pixel* output_color = new image_pixel(channels);

            for (uint32_t i = 0; i < mask.size(); i++)
            {
                int target_x = x + testX[i];
                int target_y = y + testY[i];

                if (target_y >= 0 && target_y < height && target_x >= 0 && target_x < width)
                {     
                    *(output_color) |= *(pixels[target_y * width + target_x]) & mask[i];
                }
            }
            temp[y * width + x] = output_color;
        }
    }

    for(int i = 0; i < temp.size(); i++)
    {
        *pixels[i] = *temp[i];
        delete temp[i];
    }
}

void Image::extract_word(std::string s, std::string& s1, std::string& s2)
{
    int i, mode, len;

    len = s.length();
    s1 = s2 = ""; 

    mode = 1;

    for (i = 0; i < len; i++)
    {
        if (mode == 1)
        {
            if (s[i] != ' ')
                mode = 2;
        }
        else if (mode == 2)
        {
            if (s[i] == ' ')
                mode = 3;
        }
        else if (mode == 3)
        {
            if (s[i] != ' ')
                mode = 4;
        }
        if (mode == 2)
        {
            s1 = s1 + s[i];
        }
        else if (mode == 4)
        {
            s2 = s2 + s[i];
        }
    }

    return;
}

int Image::len_trim(std::string s)
{
    int n;

    n = s.length();

    while (0 < n)
    {
        if (s[n - 1] != ' ')
            return n;
        n = n - 1;
    }

    return n;
}
