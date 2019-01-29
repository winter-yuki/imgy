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
{}


void Render::render()
{
    prep_dirs();

    for (SizeT row = 0; row < image_.rows(); ++row) {
        for (SizeT col = 0; col < image_.cols(); ++col) {
            auto ray = calc_ray_dir(row, col);
            image_(row, col) = trace_ray(ray).get_pix();
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
//    assert(image_.rows() == image_.cols()); // TODO(rows == cols)
//    Double _add_x = image_.cols() % 2 == 1 ? 0 : 0.5;
//    Double _add_y = image_.rows() % 2 == 1 ? 0 : 0.5;

//    Double _x = (col + _add_x - image_.cols() / 2.0) / image_.cols() * vport_w_;
//    Double _y = (row + _add_y - image_.rows() / 2.0) / image_.rows() * vport_h_;

//    Vector _on_vport = { _x, _y, dist_ };
//    Vector _dir      = _on_vport - pos_;
//    Vector _from     = pos_ + _dir;

////    return { _dir, _from };

    assert(image_.rows() == image_.cols()); // TODO(rows == cols)
    Double add_x = image_.cols() % 2 == 1 ? 0 : 0.5;
    Double add_y = image_.rows() % 2 == 1 ? 0 : 0.5;

    Double x = (col + add_x - image_.cols() / 2.0) / image_.cols() * vport_w_;
    Double y = (row + add_y - image_.rows() / 2.0) / image_.rows() * vport_h_;

    Vector pos_to_ = to_ * dist_;
    Vector shift_right = right_ * x;
    Vector shift_up    = up_ * y;
    Vector pos_vport   = pos_to_ + shift_up + shift_right;

//    std::cout << _dir.normalize() << " " << pos_vport.normalize() << std::endl;

    Vector from = pos_ + pos_vport;
    return { pos_vport, from };
}


Color Render::trace_ray(Ray const & ray) const
{
    Double t_min  = std::numeric_limits<Double>::max();
    Vector rez_normal;
    Color  rez_color{};

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

    if (t_min <= 0) {
        return rez_color;
    }

    Double i = count_light(ray.count(t_min), rez_normal);
    rez_color.apply_intensity(i);
    return rez_color;
}


Render::Double Render::count_light(Vector const & point,
                                   Vector const & normal) const
{
    Double intensity = 1;
    for (auto const & lts : lts_) {
        intensity += lts->light(normal, point);
    }
    return intensity;
}

} // namespace Render




































