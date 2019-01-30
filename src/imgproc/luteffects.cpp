#include "include/imgproc/luteffects.hpp"

#include <cmath>

#include "include/utility/color.hpp"


namespace ImageProcessing
{

LUTeffects::LUTeffects(IImgFile & image)
    : image_(image)
{}


void LUTeffects::negative()
{
    lut_apply(lut_negative());
}


void LUTeffects::brightness(Double coef)
{
    lut_apply(lut_brightness(coef));
}


void LUTeffects::contrast(Double a, Double b)
{
    lut_apply(lut_contrast(a, b));
}


void LUTeffects::gamma_correction(Double gamma)
{
    lut_apply(lut_gamma_correction(gamma));
}


void LUTeffects::lut_apply(LUTTable const & table)
{
    auto apply = [this, &table](SizeT row, SizeT col) {
        RawPix p = image_(row, col);
        image_(row, col) = {table.at(p.b), table.at(p.g), table.at(p.r)};
    };

    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            apply(row, col);
        }
    }
}


LUTeffects::LUTTable LUTeffects::lut_get_matr(LUTFunc const & f)
{
    LUTTable table;
    table.reserve(COLOR_MAX());
    for (SizeT i = 0; i <= COLOR_MAX(); ++i) {
        table.push_back(f(i));
    }
    return table;
}


LUTeffects::LUTTable LUTeffects::lut_negative()
{
    return lut_get_matr([](RawCP c) { return COLOR_MAX() - c; });
}


LUTeffects::LUTTable LUTeffects::lut_brightness(Double coef)
{
    return lut_get_matr([coef](RawCP c) {
        return Color::clamp_color(SizeT(c + coef * COLOR_MAX())); });
}


LUTeffects::LUTTable LUTeffects::lut_contrast(Double a, Double b)
{
    return lut_get_matr([a, b](RawCP c) {
        return Color::clamp_color(
                    SizeT(COLOR_MAX() * (c - a) / (b - a))); });
}


LUTeffects::LUTTable LUTeffects::lut_gamma_correction(Double gamma)
{
    return lut_get_matr([gamma](RawCP c) {
        return COLOR_MAX() *
                std::pow(Double(c) / COLOR_MAX(), 1.0 / gamma); });
}

} // namespace ImageProcessing
