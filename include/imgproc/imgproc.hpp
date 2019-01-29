#ifndef INCLUDE_IMGPROC_IMGPROC_HPP
#define INCLUDE_IMGPROC_IMGPROC_HPP

#include "include/file/filebase.hpp"
#include "include/imgproc/types.hpp"


namespace ImageProcessing
{

class ImgProc final {
public:
    struct PixelHSV { RawCP h, s, v; };

public:
    explicit ImgProc(IImgFile & image);
    ImgProc(ImgProc const &)             = delete;
    ImgProc & operator=(ImgProc const &) = delete;
    ImgProc(ImgProc &&)                  = delete;
    ImgProc & operator=(ImgProc &&)      = delete;

    void fill_color      (RawColor c);
    void negative        ();
    void brightness      (Double coef);
    void contrast        (Double a, Double b);
    void gamma_correction(Double gamma);

    void bayer_filter         (SizeT matr_size);
    void error_diffusion      ();
    void warp                 (Point from, Point dest);
    void free_form_deformation(Point p, SizeT right, SizeT up);

    void swap(ImgProc & other);

    SizeT rows() const { return image_.rows(); }
    SizeT cols() const { return image_.cols(); }

    static RawPix    hsv2rgb (PixelHSV p);
    static PixelHSV rgb2hsv (RawPix   p);


private:
    IImgFile & image_;
};

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_IMGPROC_HPP
