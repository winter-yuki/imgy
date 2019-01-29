#ifndef INCLUDE_RENDER_COLOR_HPP
#define INCLUDE_RENDER_COLOR_HPP

#include "include/file/types.hpp"
#include "include/render/types.hpp"


namespace Render
{

class Color final {
public:
    using ColorPart = int64_t;
    using RawCP     = ImgTypes::ColorPart;
    using Double    = RenderTypes::Double;

    static const RawCP COLOR_MAX_ = ImgTypes::COLOR_MAX_;
    static const RawCP COLOR_MIN_ = ImgTypes::COLOR_MIN_;

public:
    Color() = default;
    Color(ColorPart r, ColorPart g, ColorPart b);
    explicit Color(ImgTypes::Color c);

    void apply_intensity(Double i);

    RawCP r() const { return color_.r; }
    RawCP g() const { return color_.g; }
    RawCP b() const { return color_.b; }

    RawCP set_r(ColorPart r);
    RawCP set_g(ColorPart g);
    RawCP set_b(ColorPart b);

    ImgTypes::Pixel get_pix() const;

    static RawCP clamp_color(ColorPart c);

private:
    ImgTypes::Color color_{};
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

inline Color::Color(ImgTypes::Color c)
    : color_(c)
{}


inline void Color::apply_intensity(Double i) {
    color_.r = clamp_color(ColorPart(color_.r * i));
    color_.g = clamp_color(ColorPart(color_.g * i));
    color_.b = clamp_color(ColorPart(color_.b * i));
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

inline ImgTypes::Pixel Color::get_pix() const {
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

} // namespace Render


#endif // INCLUDE_RENDER_COLOR_HPP
