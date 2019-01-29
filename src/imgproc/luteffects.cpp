#include "include/imgproc/luteffects.hpp"


void LUTeffects::negative()
{
    lut_apply(lut_negative());
}


void LUTeffects::brightness(Param coef)
{
    lut_apply(lut_brightness(coef));
}


void LUTeffects::contrast(Param a, Param b)
{
    lut_apply(lut_contrast(a, b));
}


void LUTeffects::gamma_correction(Param gamma)
{
    lut_apply(lut_gamma_correction(gamma));
}


void LUTeffects::lut_apply(LUTTable const & table)
{
    auto apply = [this, &table](SizeT row, SizeT col) {
        Pixel p = image_(row, col);
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
    table.reserve(COLOR_MAX_);
    for (SizeT i = 0; i <= COLOR_MAX_; ++i) {
        table.push_back(f(i));
    }
    return table;
}


LUTeffects::LUTTable LUTeffects::lut_negative()
{
    return lut_get_matr([](LUTeffects::ColorPart c) {
        return LUTeffects::COLOR_MAX_ - c; });
}


LUTeffects::LUTTable LUTeffects::lut_brightness(Param coef)
{
    return lut_get_matr([coef](LUTeffects::ColorPart c) {
        return LUTeffects::clamp_color(int(c + coef * LUTeffects::COLOR_MAX_)); });
}


LUTeffects::LUTTable LUTeffects::lut_contrast(Param a, Param b)
{
    return lut_get_matr([a, b](LUTeffects::ColorPart c) {
        return LUTeffects::clamp_color(
                    int(LUTeffects::COLOR_MAX_ * (c - a) / (b - a))); });
}


LUTeffects::LUTTable LUTeffects::lut_gamma_correction(Param gamma)
{
    return lut_get_matr([gamma](LUTeffects::ColorPart c) {
        return LUTeffects::COLOR_MAX_ *
                std::pow(double(c) / LUTeffects::COLOR_MAX_, 1.0 / gamma); });
}
