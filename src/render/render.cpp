#include <functional>
#include <utility>

#include "include/render/render.hpp"


namespace Render
{

Render::Render(IImgFile & image, Objects objs)
    : image_(image)
    , objs_ (std::move(objs))
{}


void Render::render()
{
    // WARNING normalize return value reference
    cam_up_.normalize();
    pos_scr_ = (cam_to_ - cam_pos_).normalize() * dist_;
    right_   = (cam_up_.cross(pos_scr_)).normalize();

    // TODO(odd viewport sizes)
    assert(image_.height() % 2 == 0 && image_.width() % 2 == 0);
    for (SizeT row = 0; row < image_.height(); ++row) {
        for (SizeT col = 0; col < image_.width(); ++col) {
           auto ray = calc_ray_dir(row, col);
           image_(row, col) = trace_ray(ray);
        }
    }
}


Render::Vector Render::calc_ray_dir(SizeT row, SizeT col)
{
    // TODO(prove)
    ValT   r_coef = (col + 0.5 - scr_w_ / 2) / scr_w_ * image_.width();
    Vector shift_right = right_ * r_coef;

    ValT   u_coef = (-(row + 0.5) + scr_h_ / 2) /
            scr_h_ * image_.height();
    Vector shift_up = cam_up_ * u_coef;

    Vector to_pix = pos_scr_ + shift_up + shift_right;
    Vector ray    = (cam_pos_ + to_pix).normalize();

    return ray;
}


Render::Color Render::trace_ray(Vector const & ray)
{
    ValT  t_min = std::numeric_limits<ValT>::max();
    Color color{};

    for (auto obj : objs_) {
        auto rez = obj->intersect(ray, cam_pos_);
        if (rez.first < t_min) {
            t_min = rez.first;
            color = rez.second;
        }
    }

    return color;
}

} // namespace Render
