#include "include/render/render.hpp"

#include <functional>
#include <iostream> // TODO(dbg print)
#include <limits>
#include <utility>


namespace Render
{

Render::Render(IImgFile & image, Figures figs)
    : image_(image)
    , figs_ (std::move(figs))
{}


void Render::render()
{
    prep_dirs();

    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            auto ray = calc_ray_dir(row, col);
//            std::cout << ray.get_view().D << std::endl; // TODO(dbg print)
            image_(row, col) = trace_ray(ray);
        }
    }
}


void Render::prep_dirs()
{
    up_.normalize();
    to_.normalize();
    right_ = to_.cross(up_);
    right_.normalize();
}


Ray Render::calc_ray_dir(SizeT row, SizeT col) const
{
    Vector pos_vport = to_ * dist_;
    Double r_coef = (col + 0.5 - image_.cols() / 2.0) /
            image_.cols() * vport_w_;
    Vector shift_right = right_ * r_coef;

    Double u_coef = (-(row + 0.5) + image_.rows() / 2.0) /
            image_.rows() * vport_h_;
    Vector shift_up = up_ * u_coef;

    Vector pos_vport_point = pos_vport + shift_right + shift_up;
    Vector from = pos_ + pos_vport_point;
    pos_vport_point.normalize();

    return { pos_vport_point, from };
}


Render::Color Render::trace_ray(Ray const & ray) const
{
    Double t_min = std::numeric_limits<Double>::max();
    Color  color{};

    for (auto fig : figs_) {
        auto rez = fig->intersect(ray);
        if (rez.first < t_min && rez.first > 0) {
            t_min = rez.first;
            color = rez.second;
        }
    }

    return color;
}

} // namespace Render




































