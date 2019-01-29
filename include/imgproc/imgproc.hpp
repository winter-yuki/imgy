#ifndef INCLUDE_IMGPROC_IMGPROC_HPP
#define INCLUDE_IMGPROC_IMGPROC_HPP

#include <limits>
#include <tuple>
#include <vector>

#include <eigen3/Eigen/Dense>

#include "include/file/filebase.hpp"


class ImgProc final {
public:
    using  Param  = double;
    static const SizeT N_PIVOTS_V = 3; // TODO(N_PIVOTS)
    static const SizeT N_PIVOTS_H = 3;
    using  Pivots = Eigen::Matrix<Point, N_PIVOTS_V, N_PIVOTS_H>;
    struct PixelHSV { ColorPart h, s, v; };

public:
    explicit ImgProc(IImgFile & image);
    ImgProc(ImgProc const &)             = delete;
    ImgProc & operator=(ImgProc const &) = delete;
    ImgProc(ImgProc &&)                  = delete;
    ImgProc & operator=(ImgProc &&)      = delete;

    void fill_color      (Color c);
    void negative        ();
    void brightness      (Param coef);
    void contrast        (Param a, Param b);
    void gamma_correction(Param gamma);

    void bayer_filter         (SizeT matr_size);
    void error_diffusion      ();
    void warp                 (Point from, Point dest);
    void free_form_deformation(Point p, SizeT right, SizeT up);

    void swap(ImgProc & other);

    SizeT rows() const { return image_.rows(); }
    SizeT cols() const { return image_.cols(); }

    static Pixel    hsv2rgb (PixelHSV p);
    static PixelHSV rgb2hsv (Pixel    p);


private:
    IImgFile & image_;
};


#endif // INCLUDE_IMGPROC_IMGPROC_HPP
