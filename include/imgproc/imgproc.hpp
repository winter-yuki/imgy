#ifndef INCLUDE_IMGPROC_IMGPROC_HPP
#define INCLUDE_IMGPROC_IMGPROC_HPP

#include <limits>
#include <tuple>

#include <eigen3/Eigen/Dense>

#include "include/file/imgfile.hpp"


class ImgProc final
        : public ImgTypes {
public:
    using  Param  = double;
    static const SizeT N_PIVOTS_V = 3; // TODO(N_PIVOTS)
    static const SizeT N_PIVOTS_H = 3;
    using  Pivots = Eigen::Matrix<Point, N_PIVOTS_V, N_PIVOTS_H>;
    struct PixelHSV { ColorPart h, s, v; };

    static const ColorPart COLOR_MAX_ = std::numeric_limits<ColorPart>::max();
    static const ColorPart COLOR_MIN_ = 0;
    const Pixel WHITE_ = { COLOR_MAX_, COLOR_MAX_, COLOR_MAX_ }; // TODO(static)
    const Pixel BLACK_ = { COLOR_MIN_, COLOR_MIN_, COLOR_MIN_ };

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

    SizeT rows() const { return image_.height(); }
    SizeT cols() const { return image_.width();  }

    static Pixel    hsv2rgb (PixelHSV p);
    static PixelHSV rgb2hsv (Pixel    p);


private:
    using LUTTable = std::vector<ColorPart>;
    using LUTFunc  = std::function<ColorPart (ColorPart)>;
    void lut_apply (LUTTable const & table);
    static LUTTable lut_get_matr        (LUTFunc const & f);
    static LUTTable lut_negative        ();
    static LUTTable lut_brightness      (Param coef);
    static LUTTable lut_contrast        (Param a, Param b);
    static LUTTable lut_gamma_correction(Param gamma);

    using  BayerMtr = Eigen::Array<SizeT, Eigen::Dynamic, Eigen::Dynamic>;
    static BayerMtr get_bayer_matrix(SizeT size);

    static ColorPart clamp_color(int color);

    // warp
    using ValT         = double;
    using Matrix       = Eigen::Matrix<ValT, 3, 3>;
    using PointH       = Eigen::Matrix<ValT, 1, 3>;
    using Vector       = Eigen::Matrix<ValT, 1, 3>;
    using Triangle     = Matrix;
    using TriTransform = std::vector<std::pair<Triangle, Matrix>>;
    TriTransform   get_back_mapping(Point from, Point dest);
    Matrix find_triangle(TriTransform const & map, Point p);
    static Point convert(PointH const & p) {
        return { SizeT(p[0] / p[2]), SizeT(p[1] / p[2]) }; }
    static ValT norm(Vector v) { return std::sqrt(v[0]*v[0] + v[1]*v[1]); }

    // ffd
    using PointD = Eigen::Matrix<ValT, 1, 3>;
    Pivots gen_pivots(SizeT img_w, SizeT img_h)  const;
    std::tuple<SizeT /*height*/, SizeT /*width*/>
    get_new_size(Point p, SizeT right, SizeT up) const;
    void shift_pivots(Point p, SizeT right, SizeT up);
    void ffd_map(ImgProc const & from);
    Pixel & get_param_point(ValT v, ValT u);
    Pixel & get_param_point(ValT v, ValT u) const;
    PointD  bilinear_interp(ImgTypes::Point p,
                            Param u /*p00 - p01*/, Param v /*p00 - p10*/) const;
    PointD bilinear_interp(PointD const & p00, PointD const & p01,
                           PointD const & p10, PointD const & p11,
                           Param u /*p00 - p01*/, Param v /*p00 - p10*/) const;

private:
    IImgFile & image_;
    Pivots     ffd_pivots_;
};


#endif // INCLUDE_IMGPROC_IMGPROC_HPP
