#include "image_pixel.h"
#include <numeric>
#include <tuple>
#include <string>

/**
 * dla jednokanalowych obrazow zwraca wartosc piksela, dla obrazu RGB zwraca srednia arytmetyczna wartosci RGB
 * @param const reference do vector<uinty16_t>
 * @return double - srednia arytmetyczna
 */
double get_average(const std::vector<uint16_t>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), 0.0, [&](double a, double b){return a + b / vec.size(); });
}

/**
 * copy constructor
 * @param const reference do pixela ktorego chcemy skopiowac
 * @return image_pixel - kopia pixelu umieszczonego w parametrze
 */
image_pixel::image_pixel(const image_pixel& obj)
{
    pixel_ = obj.pixel_;
    size_ = obj.size_;
}

/**
 * ustaw wszystkie kanaly pixela na jedna wartosc. np dla RGB ustawi R = wartosc, G = wartosc, B = wartosc
 * @param const reference do int - ustaw wartosc pixela na wartosc tego parametru
 */
void image_pixel::set_pixel(const int& t)
{
    for (int i = 0; i < pixel_.size(); i++)
    {
        pixel_[i] = (uint16_t)t;
    }
}

/**
 * przeciazony output stream '<<'
 * @param reference do ostream
 * @param const reference do image_pixel - pixel, ktory chcemy umiescic w strumieniu
 * @return reference do ostream
 */
std::ostream& operator<<(std::ostream& os, const image_pixel& ip)
{
    if(ip.size_ == 3)
    {
        os << ip.pixel_[0] << ' ' << ip.pixel_[1] << ' ' << ip.pixel_[2] << ' ';
    }
    else
    {
        os << ip.pixel_[0] << ' ';
    }
    return os;
}

/**
 * przeciazony input stream '>>'
 * @param reference do istream
 * @param reference do image_pixel do ktorego chcemy dodac dane ze strumienia
 * @return reference do istream
 */
std::istream& operator>> (std::istream& in, image_pixel& ip)
{
    if(ip.size_ == 3)
    {
        std::string tmp;
        for(int i = 0; i < 3; i++)
        {
            uint16_t val;
            in >> tmp;
            val = (uint16_t)std::stoi(tmp);
            ip.pixel_[i] = val;
        }
    }
    else
    {
        std::string tmp;
        uint16_t val;
        in >> tmp;
        val = (uint16_t)std:: stoi(tmp);
        ip.pixel_[0] = val;
    }
    return in;
}
/**
 * przeciazony operator '+', dodaje dwa pixele do siebie
 * @param image_pixel - lewa strona dodawania
 * @param const reference do image_pixel - prawa strona dodawania
 * @return image_pixel - suma pixeli
 */
image_pixel operator+ (image_pixel lhs, const image_pixel& rhs)
{
    for (int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] + rhs.pixel_[i];
    }

    return lhs;
}
/**
 * przeciazony operator '-', odejmuje dwa pixele od siebie
 * @param image_pixel, lewa strona odejmowania
 * @param const reference do image_pixel, prawa strona odejmowania
 * @return image_pixel = roznica pixeli
 */
image_pixel operator- (image_pixel lhs, const image_pixel& rhs)
{
    for (int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] - rhs.pixel_[i];
    }

    return lhs;
}
/**
 * przeciazony operator '*', mnozy dwa pixele ze soba
 * @param image_pixel, lewa strona mnozenia
 * @param const reference do image_pixel, prawa strona mnozenia
 * @return image_pixel = iloczyn pixeli
 */
image_pixel operator * (image_pixel lhs, const image_pixel& rhs)
{
    for (int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] * rhs.pixel_[i];
    }

    return lhs;
}

image_pixel operator / (image_pixel lhs, const image_pixel& rhs)
{
    for (int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] / rhs.pixel_[i];
    }

    return lhs;
}
/**
 * przeciazony operator '<', porownuje rozmiary pixeli
 * @param const reference do image_pixel, lewa strona porownania
 * @param const reference do image_pixel, prawa strona porownania
 * @return bool = true jesli prawy pixel jest wiekszy
 */
bool operator< (const image_pixel& lhs, const image_pixel& rhs)
{
    auto right = get_average(rhs.pixel_);
    auto left = get_average(lhs.pixel_);

    return left < right;
}
/**
 * przeciazony operator '>', porownuje rozmiary pixeli
 * @param const reference do image_pixel, lewa strona porownania
 * @param const reference do image_pixel, prawa strona porownania
 * @return bool = true jesli lewy pixel jest wiekszy
 */
bool operator> (const image_pixel& lhs, const image_pixel& rhs)
{
    return rhs < lhs;
}
/**
 * przeciazony operator '+=', przypisuje sume pixela z innym pixelem do siebie samego
 * @param const reference do image_pixel, prawa strona sumy
 * @return reference do image_pixel
 */
image_pixel& image_pixel::operator+= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] += rhs.pixel_[i];
    }
    return *this;
}
/**
 * przeciazony operator '-=', przypisuje roznice pixela z lewej strony od pixela z prawej strony do siebie samego
 * @param const reference do image_pixel, prawa strona roznicy
 * @return reference do image_pixel
 */
image_pixel& image_pixel::operator-= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] -= rhs.pixel_[i];
    }
    return *this;
}

/**
 * przeciazony operator '*=', przypisuje iloczony pixela z lewej strony z pixelem z prawej strony do siebie samego
 * @param const reference do image_pixel, prawa strona iloczynu
 * @return reference do image_pixel
 */
image_pixel& image_pixel::operator*= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] = pixel_[i] * rhs.pixel_[i];
    }
    return *this;
}

/**
 * metoda wylaczona
 * przeciazony operator '/=', przypisuje iloraz pixela z lewej strony przez pixel z prawej strony do siebie samego
 * @param const reference do image_pixel, prawa strona ilorazu
 * @return reference do image_pixel
 */
// image_pixel& image_pixel::operator/= (const image_pixel& rhs)
// {
//     for(int i = 0; i < size_; i++)
//     {
//         pixel_[i] = pixel_[i] / rhs.pixel_[i];
//     }
//     return *this;
// }

/**
 * metoda wylaczona
 * przeciazony operator '/=', przypisuje iloraz pixela z lewej strony przez skalar z prawej strony do siebie samego
 * @param const reference do skalara, prawa strona ilorazu
 * @return reference do image_pixel
 */
// template <typename T>
// image_pixel& image_pixel::operator/= (const T & rhs)
// {
//     for (int i = 0; i < pixel_.size(); i++)
//     {
//         pixel_[i] = (uint16_t)(pixel_[i] / rhs);
//     }

//     return *this;
// }


/**
 * przeciazony operator '&=', przypisuje logiczny AND pixela z lewej strony z pixelem z prawej strony do siebie samego
 * @param const reference do image_pixel, prawa strona logicznego AND
 * @return reference do image_pixel
 */
image_pixel& image_pixel::operator&= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] = pixel_[i] && rhs.pixel_[i];
    }
    return *this;
}

image_pixel& image_pixel::operator|= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] = pixel_[i] || rhs.pixel_[i];
    }
    return *this;
}

template <typename T>
image_pixel operator+ (image_pixel lhs, const T& s)
{
    for(int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] + (uint16_t)s;
    }
    return lhs;
}

// template <typename T>
// image_pixel operator- (image_pixel lhs, const T& s)
// {
//     for(int i = 0; i < lhs.size_; i++)
//     {
//         lhs.pixel_[i] = lhs.pixel_[i] - (uint16_t)s;
//     }
//     return lhs;
// }

// template <typename T>
// image_pixel operator- (const T& s, image_pixel rhs)
// {
//     for(int i = 0; i < rhs.size_; i++)
//     {
//         rhs.pixel_[i] = (uint16_t)s - rhs.pixel_[i] ;
//     }
//     return rhs;
// }

// template <typename T>
// image_pixel operator* (image_pixel lhs, const T& s)
// {
//     for(int i = 0; i < lhs.size_; i++)
//     {
//         lhs.pixel_[i] = lhs.pixel_[i] * s;
//     }
//     return lhs;
// }

template <typename T>
image_pixel operator/ (image_pixel lhs, const T& s)
{
    for(int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] / s;
    }
    return lhs;
}

// template <typename T>
// image_pixel operator& (image_pixel lhs, const T &s)
// {
//     for(int i = 0; i < lhs.size_; i++)
//     {
//         lhs.pixel_[i] = lhs.pixel_[i] & s;
//     }
//     return lhs;
// }

// template <typename T>
// image_pixel operator&(image_pixel lhs, T const &s)
// {
//     for(int i = 0; i < lhs.size_; i++)
//     {
//         lhs.pixel_[i] = lhs.pixel_[i] & s;
//     }
//     return lhs;
// }

// template <typename T>
// bool image_pixel::operator< (const T & s)
// {
//     bool is_T_larger = false;
//     for(int i = 0; i < size_; i++)
//     {
//         if (pixel_[i] < (uint16_t)s) is_T_larger = true;
//     }
//     return is_T_larger;
// }

// template <typename T>
// bool image_pixel::operator> (const T & s)
// {
//     bool is_T_smaller = false;
//     for(int i = 0; i < size_; i++)
//     {
//         if (pixel_[i] > (uint16_t)s) is_T_smaller = true;
//     }
//     return is_T_smaller;
// }
