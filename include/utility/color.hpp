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

    friend Color operator+(Color const & a, Color const & b);
    friend Color operator-(Color const & a, Color const & b);
    friend Color operator*(Double x, Color const & c);
    friend Color operator*(Color const & c, Double x);

private:
    struct { ColorPart r, g, b; } color_{};
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
    : color_({r, g, b})
{}

inline Color::Color(IImgFile::RawColor c)
    : color_{c.r, c.g, c.b}
{}


inline void Color::apply_intensity(Double i) {
    color_.r = ColorPart(color_.r * i);
    color_.g = ColorPart(color_.g * i);
    color_.b = ColorPart(color_.b * i);
}


inline Color::Double Color::get_intermediate_color() const {
    return Double(color_.r + color_.g + color_.b) / 3;
}


inline Color::RawCP Color::set_r(ColorPart r) {
    color_.r = r;
    return clamp_color(color_.r);
}

inline Color::RawCP Color::set_g(ColorPart g) {
    color_.g = g;
    return clamp_color(color_.g);
}

inline Color::RawCP Color::set_b(ColorPart b) {
    color_.b = b;
    return clamp_color(color_.b);
}

inline IImgFile::Pixel Color::get_pix() const {
    IImgFile::Pixel p{};
    p.r = clamp_color(color_.r);
    p.g = clamp_color(color_.g);
    p.b = clamp_color(color_.b);
    return p;
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


inline Color operator+(Color const & a, Color const & b) {
    Color c;
    c.color_.b = a.color_.b + b.color_.b;
    c.color_.g = a.color_.g + b.color_.g;
    c.color_.r = a.color_.r + b.color_.r;
    return c;
}

inline Color operator-(Color const & a, Color const & b) {
    Color c;
    c.color_.b = a.color_.b - b.color_.b;
    c.color_.g = a.color_.g - b.color_.g;
    c.color_.r = a.color_.r - b.color_.r;
    return c;
}

inline Color operator*(Color::Double x, Color const & c) {
    Color new_c;
    new_c.color_.b = Color::ColorPart(x * c.color_.b);
    new_c.color_.g = Color::ColorPart(x * c.color_.g);
    new_c.color_.r = Color::ColorPart(x * c.color_.r);
    return c;
}

inline Color operator*(Color const & c, Color::Double x) {
    return x * c;
}


#endif // INCLUDE_RENDER_COLOR_HPP
