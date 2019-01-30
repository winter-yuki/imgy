#include "include/render/render.hpp"

#include <functional>
#include <iostream> // TODO(dbg print)
#include <limits>
#include <utility>


namespace Render
{

Render::Render(IImgFile & image, Figures figs, Lights lts)
    : image_(image)
    , figs_ (std::move(figs))
    , lts_  (std::move(lts))
{
    set_vport_coef(vport_coef_);
    prep_dirs();
}


void Render::render()
{
    prep_dirs(); // TODO (change to asserts)

    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            auto ray = calc_ray_dir(row, col);
            image_(row, col) = trace_ray(ray).get_pix();
        }
    }
}


Color Render::get_bg_color  () const
{
    return background_color_;
}


Double Render::get_vport_coef() const
{
    return vport_coef_;
}


Double Render::get_vport_dist() const
{
    return dist_;
}


Vector const & Render::get_up() const
{
    return up_;
}


Vector const & Render::get_to() const
{
    return to_;
}


Vector const & Render::get_pos() const
{
    return pos_;
}


Render::Figures const & Render::get_figs() const
{
    return figs_;
}


Render::Lights const & Render::get_lights() const
{
    return lts_;
}


void Render::set_bg_color(Color c)
{
    background_color_ = c;
    fill_color(c);
}


void Render::set_vport_coef(Double coef)
{
    vport_coef_ = coef;
    vport_h_ = image_.rows() * vport_coef_;
    vport_w_ = image_.cols() * vport_coef_;
}


void Render::set_vport_dist(Double dist)
{
    dist_ = dist;
}


void Render::set_up(Vector const & up)
{
    up_ = up;
    prep_dirs();
}


void Render::set_to(Vector const & to)
{
    to_ = to;
    prep_dirs();
}


void Render::set_pos(Vector const & pos)
{
    pos_ = pos;
    prep_dirs();

    for (auto fig : figs_) {
        fig->set_cam_pos(pos);
    }
}


void Render::set_figs(Figures && figs)
{
    figs_ = figs;
}


void Render::set_lights(Lights && lts)
{
    lts_ = lts;
}


void Render::prep_dirs()
{
    view_ = to_ - pos_;
    view_.normalize();

    right_ = view_.cross(up_);
    right_.normalize();

    up_ = right_.cross(view_);
    up_.normalize();
}


void Render::fill_color(Color color)
{
    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            image_(row, col) = color.get_pix();
        }
    }
}


Ray Render::calc_ray_dir(SizeT row, SizeT col) const
{
    Double add_x = image_.cols() % 2 == 1 ? 0 : 0.5;
    Double add_y = image_.rows() % 2 == 1 ? 0 : 0.5;

    Double x = (col + add_x - image_.cols() / 2.0) / image_.cols() * vport_w_;
    Double y = (row + add_y - image_.rows() / 2.0) / image_.rows() * vport_h_;

    Vector pos_to_ = view_ * dist_;
    Vector shift_right = right_ * x;
    Vector shift_up    = up_ * y;
    Vector pos_vport   = pos_to_ + shift_up + shift_right;

    Vector from = pos_ + pos_vport;
    return { pos_vport, from };
}


Color Render::trace_ray(Ray const & ray) const
{
    Double t_min  = NO_INTERSECT();
    Vector rez_normal;
    Color  rez_color = background_color_;

    for (auto const & fig : figs_) {
        Double t{};
        Vector normal{};
        Color  color{};
        std::tie(t, normal, color) = fig->intersect(ray);

        if (t < t_min && t > 0) {
            t_min      = t;
            rez_normal = normal;
            rez_color  = color;
        }
    }

    if (t_min <= 0 || std::abs(t_min - NO_INTERSECT()) < EPSILON()) {
        return rez_color;
    }

    Double i = count_light(ray.count(t_min), rez_normal);
    rez_color.apply_intensity(i);
    return rez_color;
}


Double Render::count_light(Vector const & point,
                           Vector const & normal) const
{
    Double intensity = 0;
    for (auto const & lts : lts_) {
        intensity += lts->light(normal, point);
    }
    return intensity;
}

} // namespace Render




































