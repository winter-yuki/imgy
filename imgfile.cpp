#include "imgfile.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <utility>


ImgBmp::ImgBmp(std::string const & filename)
{
    // TODO(file optimize)
    std::ifstream ifs;
    ifs.open(filename.c_str(), std::ios::binary);
    ifs.exceptions(std::ios::badbit | std::ios::failbit);

    // Read bmp header
    static_assert(BMP_HEADER_SIZE_ == sizeof (bmp_header_),
                  "Struct pack doesn't work");
    ifs.read(reinterpret_cast<char *>(&bmp_header_), sizeof (bmp_header_));

    padding_ = calc_padding(bmp_header_);

    image_ = ImageMtr(bmp_header_.height, bmp_header_.width);
    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            Pixel tmp{};
            ifs.read(reinterpret_cast<char *>(&tmp), sizeof (tmp));
            image_(row, col) = tmp;
        }
        // Padding
        ByteT dev_null[PIX_SIZE_];
        assert(padding_ < PIX_SIZE_);
        ifs.read(reinterpret_cast<char *>(dev_null), padding_);
    }
}


void ImgBmp::print(std::string const & filename)
{
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ios::binary);
    ofs.exceptions(std::ios::badbit | std::ios::failbit);

    ofs.write(reinterpret_cast<char *>(&bmp_header_), sizeof (bmp_header_));

    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            ofs.write(reinterpret_cast<char *>(&image_(row, col)), PIX_SIZE_);
        }
        ByteT space[PIX_SIZE_];
        assert(padding_ < PIX_SIZE_);
        ofs.write(reinterpret_cast<char *>(space), padding_);
    }
}


void ImgBmp::swap(IImgFile & other)
{
    auto & o_bmp = dynamic_cast<ImgBmp &>(other);
    std::swap(bmp_header_, o_bmp.bmp_header_);
    std::swap(padding_,    o_bmp.padding_);
    std::swap(image_,      o_bmp.image_);
}


ImgBmp * ImgBmp::clone()
{
    return new ImgBmp(*this);
}


ImgBmp::Pixel & ImgBmp::operator()(SizeT row, SizeT col)
{
    return image_(row, col);
}


ImgBmp::Pixel & ImgBmp::operator()(Point p)
{
    return image_(p.y, p.x);
}


ImgBmp::Pixel const & ImgBmp::operator()(SizeT row, SizeT col) const
{
    return image_(row, col);
}


ImgBmp::Pixel const & ImgBmp::operator()(Point p) const
{
    return image_(p.y, p.x);
}


ImgBmp::SizeT ImgBmp::width()  const
{
    return image_.cols();
}


ImgBmp::SizeT ImgBmp::height() const
{
    return image_.rows();
}


ImgBmp::ImgBmp(ImgBmp const & other)
    : bmp_header_(other.bmp_header_)
    , padding_   (other.padding_)
    , image_     (other.image_)
{ }


ImgBmp::SizeT ImgBmp::calc_padding(BmpHeader const & header) const
{
    const SizeT real_width   = PIX_SIZE_ * header.width;
    const SizeT needed_width = real_width + real_width % ALIGNMENT_;
    const SizeT padding      = needed_width - real_width;
    assert((padding - real_width) % ALIGNMENT_ == 0);
    return padding;
}
















