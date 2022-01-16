#pragma once 

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "image_pixel.h"

class Image
{
private:
    int width;
    int height;
    int max_pixel_value;
    int channels;
    std::vector<image_pixel*> pixels;
    std::string magic_number;
public:
    Image(const std::string& filename);
    ~Image();

    // pomocnicze metody do ladowania obrazu do pamieci
    void ReadHeader(std::ifstream& input);
    void ReadPixels(std::ifstream& input);

    // metody niebinarnych obrazow
    void ResizeImage(int new_width, int new_height);
    void RotateImage(float ang);
    void BinaryImage(const short& treshold);
    void ApplyBlur(const std::string& method);
    void ReduceNoise();
    void NegativeImage();
    void GradientImage(const std::string& method);

    // metody binarynch obrazow
    void ErodeImage();
    void DilateImage();

    // zapisz obraz
    void SaveImage(const std::string& filename);

    // metody wewnętrzne
    int len_trim(std::string s);
    void extract_word(std::string s, std::string& s1, std::string& s2);

    template<typename T>
    void BubbleSort(std::vector<T>& vec);
};