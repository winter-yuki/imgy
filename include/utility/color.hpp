#ifndef INCLUDE_RENDER_COLOR_HPP
#define INCLUDE_RENDER_COLOR_HPP

#include "include/file/filebase.hpp"


class Color final {
public:
    using ColorPart = int64_t;
    using RawCP     = IImgFile::ColorPart;
    using Double    = double;

    static const RawCP COLOR_MAX_ = IImgFile::COLOR_MAX_;
    static const RawCP COLOR_MIN_ = IImgFile::COLOR_MIN_;

public:
    Color() = default;
    Color(ColorPart r, ColorPart g, ColorPart b);
    explicit Color(IImgFile::RawColor c);

    void apply_intensity(Double i);

    Double get_intermediate_color() const;

    RawCP r() const { return color_.r; }
    RawCP g() const { return color_.g; }
    RawCP b() const { return color_.b; }

    RawCP set_r(ColorPart r);
    RawCP set_g(ColorPart g);
    RawCP set_b(ColorPart b);

    IImgFile::Pixel get_pix() const;

    static RawCP clamp_color(ColorPart c);

private:
    IImgFile::RawColor color_{};
};


inline Color BLACK() {
    static const Color c(0, 0, 0); return c;
}

inline Color WHITE() {
    static const Color c(Color::COLOR_MAX_,
                         Color::COLOR_MAX_,
                         Color::COLOR_MAX_);
    return c;
}


inline Color::Color(ColorPart r, ColorPart g, ColorPart b)
    : color_({clamp_color(b), clamp_color(g), clamp_color(r)})
{}

inline Color::Color(IImgFile::RawColor c)
    : color_(c)
{}


inline void Color::apply_intensity(Double i) {
    color_.r = clamp_color(ColorPart(color_.r * i));
    color_.g = clamp_color(ColorPart(color_.g * i));
    color_.b = clamp_color(ColorPart(color_.b * i));
}


inline Color::Double Color::get_intermediate_color() const {
    return Double(ColorPart(color_.r) + color_.g + color_.b) / 3;
}


inline Color::RawCP Color::set_r(ColorPart r) {
    color_.r = clamp_color(r);
    return color_.r;
}

inline Color::RawCP Color::set_g(ColorPart g) {
    color_.g = clamp_color(g);
    return color_.g;
}

inline Color::RawCP Color::set_b(ColorPart b) {
    color_.b = clamp_color(b);
    return color_.b;
}

inline IImgFile::Pixel Color::get_pix() const {
    return color_;
}

inline Color::RawCP Color::clamp_color(ColorPart c) {
    if (c > COLOR_MAX_) {
        return COLOR_MAX_;
    }
    if (c < COLOR_MIN_) {
        return COLOR_MIN_;
    }
    return RawCP(c);
}


#endif // INCLUDE_RENDER_COLOR_HPP
