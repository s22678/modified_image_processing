#include "image_pixel.h"
#include <numeric>
#include <tuple>

double get_average(const std::vector<uint16_t>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), 0.0, [&](double a, double b){return a + b / vec.size(); });
}

image_pixel::image_pixel(const image_pixel& obj)
{
    pixel_ = obj.pixel_;
    size_ = obj.size_;
}

void image_pixel::set_pixel(const int& t)
{
    for (int i = 0; i < pixel_.size(); i++)
    {
        pixel_[i] = (uint16_t)t;
    }
}

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

std::istream& operator>> (std::istream& in, image_pixel& ip)
{
    if(ip.size_ == 3)
    {
        std::string tmp;
        for(int i = 0; i < 3; i++)
        {
            uint16_t val;
            in >> tmp;
            val = (uint16_t)stoi(tmp);
            ip.pixel_[i] = val;
        }
    }
    else
    {
        std::string tmp;
        uint16_t val;
        in >> tmp;
        val = (uint16_t)stoi(tmp);
        ip.pixel_[0] = val;
    }
    return in;
}

image_pixel operator+ (image_pixel lhs, const image_pixel& rhs)
{
    for (int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] + rhs.pixel_[i];
    }

    return lhs;
}

image_pixel operator- (image_pixel lhs, const image_pixel& rhs)
{
    for (int i = 0; i < lhs.size_; i++)
    {
        lhs.pixel_[i] = lhs.pixel_[i] - rhs.pixel_[i];
    }

    return lhs;
}

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

bool operator< (const image_pixel& lhs, const image_pixel& rhs)
{
    // if (lhs.pixel_.size() == 3)
    // {
    //     return std::tie(lhs.pixel_[0], lhs.pixel_[1], lhs.pixel_[2])
    //         < std::tie(rhs.pixel_[0], rhs.pixel_[1], rhs.pixel_[2]); // keep the same order
    // }
    auto right = get_average(rhs.pixel_);
    auto left = get_average(lhs.pixel_);

    return left < right;
}

bool operator> (const image_pixel& lhs, const image_pixel& rhs)
{
    return rhs < lhs;
}

image_pixel& image_pixel::operator+= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] += rhs.pixel_[i];
    }
    return *this;
}

image_pixel& image_pixel::operator-= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] -= rhs.pixel_[i];
    }
    return *this;
}

image_pixel& image_pixel::operator*= (const image_pixel& rhs)
{
    for(int i = 0; i < size_; i++)
    {
        pixel_[i] = pixel_[i] * rhs.pixel_[i];
    }
    return *this;
}

// image_pixel& image_pixel::operator/= (const image_pixel& rhs)
// {
//     for(int i = 0; i < size_; i++)
//     {
//         pixel_[i] = pixel_[i] / rhs.pixel_[i];
//     }
//     return *this;
// }

// template <typename T>
// image_pixel& image_pixel::operator/= (const T & rhs)
// {
//     for (int i = 0; i < pixel_.size(); i++)
//     {
//         pixel_[i] = (uint16_t)(pixel_[i] / rhs);
//     }

//     return *this;
// }

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
