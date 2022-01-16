#pragma once

#include <vector>
#include <fstream>

struct image_pixel;

std::ostream& operator<<(std::ostream& os, const image_pixel& ip);
std::istream& operator>> (std::istream& in, image_pixel& ip);

struct image_pixel
{
    std::vector<uint16_t> pixel_;
    int size_;

    image_pixel() : size_(0) {};
    image_pixel(int size) : size_(size) { for (int i = 0; i < size; i++) { pixel_.push_back(0); } };
    image_pixel(int size, uint16_t R, uint16_t G, uint16_t B) : size_(size) { pixel_.reserve(size); pixel_.push_back(R); pixel_.push_back(G); pixel_.push_back(B); };
    image_pixel(int size, uint16_t gray_scale) : size_(size) { pixel_.reserve(size); pixel_.push_back(gray_scale); };
    image_pixel(const image_pixel& obj);

    void set_pixel(const int&); // ustawia wszystkie wartosci piksela na dana wartosc

    friend image_pixel operator+ (image_pixel, const image_pixel&); // Przeciazony operator + dodajacy elementy skladowe wektora do siebie v1[0] + v2[0], v1[1] + v2[1] itd.
    friend image_pixel operator- (image_pixel, const image_pixel&); // Przeciazony operator − odejmujacy elementy skladowe wektora od siebie v1[0] - v2[0], v1[1] - v2[1] itd.

    friend image_pixel operator* (image_pixel, const image_pixel&); // Przeciazony operator * mnozacy elementy skladowe wektora przez siebie v1[0] * v2[0], v1[1] * v2[1] itd
    friend image_pixel operator/ (image_pixel, const image_pixel&); // Przeciazony operator / dzielacy elementy skladowe wektora przez siebie v1[0] / v2[0], v1[1] / v2[1] it.

    friend bool operator< (const image_pixel&, const image_pixel&); // Przeciazony operator < porownuje elementy skladowe wektora: v1[0] < v2[0], v1[1] < v2[1] itd.
    friend bool operator> (const image_pixel&, const image_pixel&); // Przeciazony operator > porownuje elementy skladowe wektora: v1[0] > v2[0], v1[1] > v2[1] itd.

    image_pixel& operator+= (const image_pixel&);  // Przeciazony operator += dodajacy elementy skladowe wektora do siebie: v1[0] = v1[0] + v2[0], v1[1] = v1[1] + v2[1] itd.
    image_pixel& operator-= (const image_pixel&);  // Przeciazony operator -= odejmujacy elementy skladowe wektora do siebie: v1[0] = v1[0] - v2[0], v1[1] = v1[1] - v2[1] itd.

    image_pixel& operator*= (const image_pixel&);  // Przeciazony operator *= mnozacy elementy skladowe wektora ze soba: v1[0] = v1[0] * v2[0], v1[1] = v1[1] * v2[1] itd.
    image_pixel& operator/= (const image_pixel& rhs)  // Przeciazony operator /= dzielacy elementy skladowe wektora przez siebie: v1[0] = v1[0] / v2[0], v1[1] = v1[1] / v2[1] itd.
    {
        for(int i = 0; i < size_; i++)
        {
            pixel_[i] = (uint16_t)(pixel_[i] / rhs.pixel_[i]);
        }
        return *this;
    }
    
    template <typename T>
    image_pixel& operator/= (const T& rhs)  //Przeciazony operator /= dzielacy elementy skladowe wektora przez skalar: v1[0] = v1[0] / 5, v1[1] = v1[1] / 5 itd.
    {
        for (int i = 0; i < pixel_.size(); i++)
        {
            pixel_[i] = (uint16_t)(pixel_[i] / rhs);
        }

        return *this;
    }
    
    image_pixel& operator&= (const image_pixel&);  // Przeciazony operator += dodajacy elementy skladowe wektora do siebie: v1[0] = v1[0] + v2[0], v1[1] = v1[1] + v2[1] itd.
    image_pixel& operator|= (const image_pixel&);  // Przeciazony operator -= odejmujacy elementy skladowe wektora do siebie: v1[0] = v1[0] - v2[0], v1[1] = v1[1] - v2[1] itd.

    template <typename T>
    friend image_pixel operator+ (image_pixel, const T &); // Przeciazony operator + dodajacy 'skalar' do wektora: v[0] + 5, v[1] + 5, v[2] + 5 itd.

    template <typename T>
    friend image_pixel operator- (image_pixel lhs, const T & s) // Przeciazony operator − odejmujacy skalar od wektora: v[0] - 5, v[1] - 5, v[2] - 5 itd.
    {
        for(int i = 0; i < lhs.size_; i++)
        {
            lhs.pixel_[i] = lhs.pixel_[i] - (uint16_t)s;
        }
        return lhs;
    }

    template <typename T>
    friend image_pixel operator- (const T& s, image_pixel rhs) // Przeciazony operator − odejmujacy skalar od wektora: v[0] - 5, v[1] - 5, v[2] - 5 itd.
    {
        for(int i = 0; i < rhs.size_; i++)
        {
            rhs.pixel_[i] = (uint16_t)s - rhs.pixel_[i] ;
        }
        return rhs;
    }

    template <typename T>
    friend image_pixel operator* (image_pixel lhs, const T& s) // Przeciazony operator * mnozacy 'skalar' z wektorem: v[0] * 5, v[1] * 5, v[2] * 5 itd.
    {
        for(int i = 0; i < lhs.size_; i++)
        {
            lhs.pixel_[i] = (uint16_t)(lhs.pixel_[i] * s);
        }
        return lhs;
    }

    template <typename T>
    friend image_pixel operator/ (image_pixel, const T &); // Przeciazony operator / dzielacy wektor przez skalar: v[0] / 5, v[1] / 5, v[2] / 5 itd.

    // template <typename T>
    // friend image_pixel operator& (image_pixel, const T &); // Przeciazony operator & "and'ujacy" wektor ze skalarem: v[0] & 5, v[1] & 5, v[2] & 5 itd.

    template <typename T>
    friend image_pixel operator&(image_pixel lhs, T const &s)
    {
        for(int i = 0; i < lhs.size_; i++)
        {
            lhs.pixel_[i] = lhs.pixel_[i] & s;
        }
        return lhs;
    } // Przeciazony operator & "and'ujacy" wektor ze skalarem: v[0] & 5, v[1] & 5, v[2] & 5 itd.

    template <typename T>
    bool operator< (const T& s)
    {
        bool is_T_larger = false;
        for(int i = 0; i < size_; i++)
        {
            if (pixel_[i] < (uint16_t)s) is_T_larger = true;
        }
        return is_T_larger;
    }    // Przeciazony operator < porownuje elementy skladowe wektora ze zmienna: v1[0] < 5, v1[1] < 5 itd.
    
    template <typename T>
    bool operator> (const T& s)
    {
        bool is_T_smaller = false;
        for(int i = 0; i < size_; i++)
        {
            if (pixel_[i] > (uint16_t)s) is_T_smaller = true;
        }
        return is_T_smaller;
    } // Przeciazony operator > porownuje elementy skladowe wektora ze zmianna: v1[0] > 5, v1[1] > 5 itd.

    friend std::ostream& operator<<(std::ostream& os, const image_pixel& dt);

    friend std::istream& operator>> (std::istream& in, const image_pixel& n);
};