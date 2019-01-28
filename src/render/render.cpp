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
    // TODO(add_x & add_y needed?)
    Double add_x = image_.cols() % 2 == 1 ? 0 : 0.5;
    Double add_y = image_.rows() % 2 == 1 ? 0 : 0.5;

    Double x = (col + add_x - image_.cols() / 2.0) / image_.cols() * vport_w_;
    Double y = (row + add_y - image_.rows() / 2.0) / image_.rows() * vport_h_;

    Vector on_vport = { x, y, dist_ };
    Vector dir      = on_vport - pos_;
    Vector from     = pos_ + dir;

    return { dir, from };
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




































