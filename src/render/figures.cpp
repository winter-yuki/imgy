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
    // TODO(FigSphere::intersect)
    auto r = ray.get_view();

    Vector OC = r.O - center_;
    Double k1 = r.D * r.D;
    Double k2 = OC * r.D;
    Double k3 = OC * OC - radius_ * radius_;
    Double diskr = k2 * k2 - k1 * k3;
    if (diskr < 0) {
        return { std::numeric_limits<Double>::max(), {0, 0, 0} };
    }

    Double sdiskr = std::sqrt(diskr);
    Double t1 = -k2 + sdiskr;
    Double t2 = -k2 - sdiskr;

    return { std::min(t1, t2), color_ };
}


FigCuboid::Intersect FigCuboid::intersect(Ray const & ray) const
{
    // TODO(FigCuboid::intersect)
}


FigPlane::Intersect FigPlane::intersect(Ray const & ray) const
{
    // TODO(FigPlane::intersect)
}

} // namespace Render



























