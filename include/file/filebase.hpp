#ifndef INCLUDE_FILE_FILEBASE_HPP
#define INCLUDE_FILE_FILEBASE_HPP

#include <iostream>
#include <limits>
#include <string>


class IImgFile {
public:
    using  SizeT     = int64_t;
    using  ColorPart = unsigned char;
    struct Point { SizeT x, y; };
    struct Pixel { ColorPart b, g, r; };
    using  RawColor = Pixel;

    static const ColorPart COLOR_MAX_ = std::numeric_limits<ColorPart>::max();
    static const ColorPart COLOR_MIN_ = std::numeric_limits<ColorPart>::min();
    constexpr static const Pixel WHITE_ = { COLOR_MAX_, COLOR_MAX_, COLOR_MAX_ };
    constexpr static const Pixel BLACK_ = { COLOR_MIN_, COLOR_MIN_, COLOR_MIN_ };

public:
    IImgFile()                             = default;
    virtual ~IImgFile()                    = default;
    IImgFile(IImgFile const &)             = delete;
    IImgFile & operator=(IImgFile const &) = delete;
    IImgFile(IImgFile &&)                  = delete;
    IImgFile & operator=(IImgFile &&)      = delete;

    virtual void print(std::string const & filename) = 0;

    virtual void resize(SizeT new_rows, SizeT new_cols) = 0;

    virtual void       swap (IImgFile & other) = 0;
    virtual IImgFile * clone()                 = 0;

    virtual Pixel       & operator()(SizeT row, SizeT col)       = 0;
    virtual Pixel       & operator()(Point p)                    = 0;
    virtual Pixel const & operator()(SizeT row, SizeT col) const = 0;
    virtual Pixel const & operator()(Point p)              const = 0;

    virtual SizeT rows() const = 0;
    virtual SizeT cols() const = 0;
};


inline bool operator==(IImgFile::Pixel p1, IImgFile::Pixel p2) {
    return p1.b == p2.b && p1.g == p2.g && p1.r == p2.r;
}

inline std::ostream & operator<<(std::ostream & os, IImgFile::Point p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


#endif // INCLUDE_FILE_FILEBASE_HPP
