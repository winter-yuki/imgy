#include "include/render/figures.hpp"


namespace Render
{

FigPlane::FigPlane(Vector const & n, Vector const & p, Color const & color,
                   Vector const & cam_pos, Material m)
    : n_    (n.normalized())
    , p_    (p)
    , d_    (-(n_ * p_))
    , color_(color)
    , m_(m)
{
    // Direct normal to cam
    set_cam_pos(cam_pos);
}


Intersect FigPlane::intersect(Ray const & ray) const
{
    auto r = ray.get_view();
    Double den = n_ * r.D;
    if (std::abs(den) < EPSILON()) {
        return NO_INTERSECT();
    }

    Double t = -(n_ * r.O + d_) / den;
    return { t, n_, color_, m_ };
}


void FigPlane::set_cam_pos(Vector const & new_pos)
{
    if (count(new_pos) < -EPSILON()) {
        n_ = n_ * -1;
    }
}


Double FigPlane::count(Vector const & point)
{
    return n_ * point + d_;
}

} // namespace Render
