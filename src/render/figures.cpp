#include "include/render/figures.hpp"

#include <cmath>
#include <limits>


namespace Render
{

FigSphere::FigSphere(Vector const & center, Double radius, Color color)
    : center_(center)
    , radius_(radius)
    , color_ (color)
{}


FigSphere::Intersect FigSphere::intersect(Ray const & ray) const
{
    auto r = ray.get_view();

    Vector OC = r.O - center_;
    Double k1 = r.D * r.D;
    Double k2 = OC * r.D;
    Double k3 = OC * OC - radius_ * radius_;
    Double diskr = k2 * k2 - k1 * k3;
    if (diskr < 0) {
        return { NO_INTERSECT, BLACK_ };
    }

    Double sdiskr = std::sqrt(diskr);
    Double t1 = -k2 + sdiskr;
    Double t2 = -k2 - sdiskr;

    return { std::min(t1, t2), color_ };
}


FigBox::Intersect FigBox::intersect(Ray const & ray) const
{
    // TODO(FigCuboid::intersect)
}


FigPlane::FigPlane(Vector const & n, Vector const & p, Color color)
    : n_    (n)
    , p_    (p)
    , d_    (-(n_ * p_))
    , color_(color)
{}


FigPlane::Intersect FigPlane::intersect(Ray const & ray) const
{
    auto r = ray.get_view();
    Double den = n_ * r.D;
    if (std::abs(den) < EPSILON) {
        return { NO_INTERSECT, BLACK_ };
    }

    Double t = -(n_ * r.O + d_) / den;
    return { t, color_ };
}

} // namespace Render



























