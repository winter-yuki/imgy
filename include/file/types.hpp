#ifndef INCLUDE_FILE_TYPES_HPP
#define INCLUDE_FILE_TYPES_HPP

#include <cinttypes>
#include <iostream>
#include <limits>


class ImgTypes {
public:
    using  SizeT     = int64_t;
    using  ColorPart = unsigned char;
    struct Point { SizeT x, y; };
    struct Pixel { ColorPart b, g, r; };
    using  Color = Pixel;

public:
    static const ColorPart COLOR_MAX_ = std::numeric_limits<ColorPart>::max();
    static const ColorPart COLOR_MIN_ = 0;
    const Pixel WHITE_ = { COLOR_MAX_, COLOR_MAX_, COLOR_MAX_ }; // TODO(static)
    const Pixel BLACK_ = { COLOR_MIN_, COLOR_MIN_, COLOR_MIN_ };
};


inline bool operator==(ImgTypes::Pixel p1, ImgTypes::Pixel p2) {
    return p1.b == p2.b && p1.g == p2.g && p1.r == p2.r;
}


inline std::ostream & operator<<(std::ostream & os, ImgTypes::Point p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


#endif // INCLUDE_FILE_TYPES_HPP
