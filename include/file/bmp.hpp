#ifndef INCLUDE_FILE_BMP_HPP
#define INCLUDE_FILE_BMP_HPP

#include <string>
#include <vector>

#include <eigen3/Eigen/Core>

#include "include/file/filebase.hpp"


class ImgBmp final
        : public IImgFile {
public:
    ImgBmp();
    explicit ImgBmp(std::string const & filename);
    ImgBmp(SizeT cols, SizeT rows);

    void print(std::string const & filename) override;

    void resize(SizeT new_rows, SizeT new_cols) override;

    void     swap(IImgFile & other) override;
    ImgBmp * clone()                override;

    Pixel       & operator()(SizeT row, SizeT col)       override;
    Pixel       & operator()(Point p)                    override;
    Pixel const & operator()(SizeT row, SizeT col) const override;
    Pixel const & operator()(Point p)              const override;

    SizeT rows() const override;
    SizeT cols() const override;


private:
    using ImageMtr  = Eigen::Array<Pixel, Eigen::Dynamic, Eigen::Dynamic>;
    using ByteT     = unsigned char;
    struct __attribute__((packed)) BmpHeader {
        ByteT    space1[18];
        uint32_t width;
        uint32_t height;
        ByteT    space2[28];
    };

    static const SizeT BMP_HEADER_SIZE_ = 54; // sizeof(bmp_header_)
    static const SizeT ALIGNMENT_       = 4;
    static const SizeT PIX_SIZE_        = 3;

    ImgBmp(ImgBmp const & other);
    SizeT calc_padding(BmpHeader const & header) const;

private:
    BmpHeader bmp_header_{};
    SizeT     padding_   {};
    ImageMtr  image_;
};


#endif // INCLUDE_FILE_BMP_HPP
