#ifndef INCLUDE_FILE_TYPES_HPP
#define INCLUDE_FILE_TYPES_HPP

#include <cinttypes>
#include <iostream>


class ImgTypes {
public:
    using  SizeT     = int64_t;
    using  ColorPart = unsigned char;
    struct Point { SizeT x, y; };
    struct Pixel { ColorPart b, g, r; };
    using  Color = Pixel;
};


inline bool operator==(ImgTypes::Pixel p1, ImgTypes::Pixel p2) {
    return p1.b == p2.b && p1.g == p2.g && p1.r == p2.r;
}


inline std::ostream & operator<<(std::ostream & os, ImgTypes::Point p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


#endif // INCLUDE_FILE_TYPES_HPP