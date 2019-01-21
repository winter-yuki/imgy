#ifndef INCLUDE_FILE_IMGFILE_HPP
#define INCLUDE_FILE_IMGFILE_HPP

#include <string>
#include <vector>

#include <eigen3/Eigen/Core>


class ImgTypes {
public:
    using  SizeT     = int64_t;
    using  ColorPart = unsigned char;
    struct Point { SizeT x, y; };
    struct Pixel { ColorPart b, g, r; };
    using  Color = Pixel;
};


class IImgFile : public ImgTypes {
public:
    IImgFile()                             = default;
    virtual ~IImgFile()                    = default;
    IImgFile(IImgFile const &)             = delete;
    IImgFile & operator=(IImgFile const &) = delete;
    IImgFile(IImgFile &&)                  = delete;
    IImgFile & operator=(IImgFile &&)      = delete;

    virtual void print(std::string const & filename) = 0;

    virtual void       swap (IImgFile & other) = 0;
    virtual IImgFile * clone()                 = 0;

    virtual Pixel       & operator()(SizeT row, SizeT col)       = 0;
    virtual Pixel       & operator()(Point p)                    = 0;
    virtual Pixel const & operator()(SizeT row, SizeT col) const = 0;
    virtual Pixel const & operator()(Point p)              const = 0;

    virtual SizeT width()  const = 0;
    virtual SizeT height() const = 0;
};


class ImgBmp final : public IImgFile {
public:
    explicit ImgBmp(std::string const & filename);
    ImgBmp(SizeT width, SizeT height);

    void print(std::string const & filename) override;

    void     swap(IImgFile & other) override;
    ImgBmp * clone()                override;

    Pixel       & operator()(SizeT row, SizeT col)       override;
    Pixel       & operator()(Point p)                    override;
    Pixel const & operator()(SizeT row, SizeT col) const override;
    Pixel const & operator()(Point p)              const override;

    SizeT width()  const override;
    SizeT height() const override;


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


inline bool operator==(IImgFile::Pixel p1, IImgFile::Pixel p2) {
    return p1.b == p2.b && p1.g == p2.g && p1.r == p2.r;
}


#endif // INCLUDE_FILE_IMGFILE_HPP
