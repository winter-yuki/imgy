#include "include/imgproc/imgproc.hpp"


ImgProc::ImgProc(IImgFile & image)
    : image_(image)
    , ffd_pivots_(gen_pivots(image.cols(), image.rows()))
{}


void ImgProc::fill_color(Color c)
{
    for (SizeT row = 0, h = rows(); row < h; ++row) {
        for (SizeT col = 0, w = cols(); col < w; ++col) {
            image_(row, col) = c;
        }
    }
}


void ImgProc::negative()
{
    lut_apply(lut_negative());
}


void ImgProc::brightness(Param coef)
{
    lut_apply(lut_brightness(coef));
}


void ImgProc::contrast(Param a, Param b)
{
    lut_apply(lut_contrast(a, b));
}


void ImgProc::gamma_correction(Param gamma)
{
    lut_apply(lut_gamma_correction(gamma));
}








ImgProc::ColorPart ImgProc::clamp_color(int color)
{
    if (color <= 0) {
        return 0;
    }

    if (color > COLOR_MAX_) {
        return COLOR_MAX_;
    }

    return color;
}





void ImgProc::swap(ImgProc & other)
{
    image_.swap(other.image_);
    std::swap(ffd_pivots_, other.ffd_pivots_);
}


ImgProc::Pixel ImgProc::hsv2rgb(PixelHSV p)
{
    if (p.s == 0) {
        return {p.v, p.v, p.v};
    }

    auto newH = p.h / 60.0;
    auto n = int(newH);
    auto frac = newH - n;
    auto c1 = ColorPart(p.v * (1.0 - p.s));
    auto c2 = ColorPart(p.v * (1.0 - p.s * frac));
    auto c3 = ColorPart(p.v * (1.0 - p.s * (1.0 - frac)));

    switch (n) {
    case 0:
        return {p.v, c3, c1};
    case 1:
        return {c2, p.v, c1};
    case 2:
        return {c1, p.v, c3};
    case 3:
        return {c1, c2, p.v};
    case 4:
        return {c3, c1, p.v};
    case 5:
        return {p.v, c1, c2};
    default:
        assert("HSV2RGV 'n' wrong value" && false);
    }
}


ImgProc::PixelHSV ImgProc::rgb2hsv(Pixel p)
{
    auto maxc  = std::max({p.b, p.g, p.r});
    auto minc  = std::min({p.b, p.g, p.r});
    auto delta = maxc - minc;

    ColorPart s{};
    if (maxc > 0) {
        s = delta / maxc;
    }

    auto v = maxc;
    ColorPart h{};
    if (s == 0) {
        h = 0;
    } else {
        auto rc = (maxc - p.r) / delta;
        auto gc = (maxc - p.g) / delta;
        auto bc = (maxc - p.b) / delta;
        if (p.r == maxc) {
            h = bc - gc;
        } else if (p.g == maxc) {
            h = 2 + rc - bc;
        } else {
            h = 4 + gc - rc;
        }
        h *= 60;
    }

    return {h, s, v};
}




























