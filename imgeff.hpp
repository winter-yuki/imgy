#ifndef IMGEFF_HPP
#define IMGEFF_HPP

#include <limits>

#include "imgfile.hpp"


class ImgEff final : public IImgTypes {
public:
    using  Param = double;
    struct PixelHSV { ColorPart h, s, v; };

    static const ColorPart COLOR_MAX_ = std::numeric_limits<ColorPart>::max();
    static const ColorPart COLOR_MIN_ = 0;
    const Pixel WHITE_ = { COLOR_MAX_, COLOR_MAX_, COLOR_MAX_ }; // TODO(static)
    const Pixel BLACK_ = { COLOR_MIN_, COLOR_MIN_, COLOR_MIN_ };

public:
    explicit ImgEff(IImgFile & image);
    ImgEff(ImgEff const &)             = delete;
    ImgEff & operator=(ImgEff const &) = delete;
    ImgEff(ImgEff &&)                  = delete;
    ImgEff & operator=(ImgEff &&)      = delete;

    void fill_color      (Color c);
    void negative        ();
    void brightness      (Param coef);
    void contrast        (Param a, Param b);
    void gamma_correction(Param gamma);

    void bayer_filter         (SizeT matr_size);
    void error_diffusion      ();
//    void warp                 (Point from, Point dest);
//    void free_form_deformation(SizeT i_point, SizeT right, SizeT up);

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

    using BayerMtr = Eigen::Array<SizeT, Eigen::Dynamic, Eigen::Dynamic>;
    static BayerMtr get_bayer_matrix(SizeT size);

    static ColorPart clamp_color(int color);

    SizeT rows() const { return image_.height(); }
    SizeT cols() const { return image_.width();  }

private:
    IImgFile & image_;
};

#endif // IMGEFF_HPP
