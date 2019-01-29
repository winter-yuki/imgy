#include "include/imgproc/imgproc.hpp"

#include <algorithm>

#include "include/imgproc/bayer.hpp"
#include "include/imgproc/errdiff.hpp"
#include "include/imgproc/ffd.hpp"
#include "include/imgproc/luteffects.hpp"
#include "include/imgproc/warp.hpp"


namespace ImageProcessing
{
// TODO(imgproc refactoring)
// TODO(imgprot testing)
ImgProc::ImgProc(IImgFile & image)
    : image_(image)
{}


void ImgProc::fill_color(RawColor c)
{
    for (SizeT row = 0, h = rows(); row < h; ++row) {
        for (SizeT col = 0, w = cols(); col < w; ++col) {
            image_(row, col) = c;
        }
    }
}


void ImgProc::negative()
{
    LUTeffects lut(image_);
    lut.negative();
}


void ImgProc::brightness(Double coef)
{
    LUTeffects lut(image_);
    lut.brightness(coef);
}


void ImgProc::contrast(Double a, Double b)
{
    LUTeffects lut(image_);
    lut.contrast(a, b);
}


void ImgProc::gamma_correction(Double gamma)
{
    LUTeffects lut(image_);
    lut.gamma_correction(gamma);
}


void ImgProc::bayer_filter(SizeT matr_size)
{
    Bayer b(image_);
    b.bayer_filter(matr_size);
}


void ImgProc::error_diffusion()
{
    ErrDiff ed(image_);
    ed.error_diffusion();
}


void ImgProc::warp(Point from, Point dest)
{
    Warp w(image_);
    w.warp(from, dest);
}


void ImgProc::free_form_deformation(Point p, SizeT right, SizeT up)
{
    FFDeffect ffd(image_);
    ffd.free_form_deformation(p, right, up);
}


RawPix ImgProc::hsv2rgb(PixelHSV p)
{
    if (p.s == 0) {
        return {p.v, p.v, p.v};
    }

    auto newH = p.h / 60.0;
    auto n = int(newH);
    auto frac = newH - n;
    auto c1 = RawCP(p.v * (1.0 - p.s));
    auto c2 = RawCP(p.v * (1.0 - p.s * frac));
    auto c3 = RawCP(p.v * (1.0 - p.s * (1.0 - frac)));

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


ImgProc::PixelHSV ImgProc::rgb2hsv(RawPix p)
{
    auto maxc  = std::max({p.b, p.g, p.r});
    auto minc  = std::min({p.b, p.g, p.r});
    auto delta = maxc - minc;

    RawCP s{};
    if (maxc > 0) {
        s = delta / maxc;
    }

    auto v = maxc;
    RawCP h{};
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

} // namespace ImageProcessing


























