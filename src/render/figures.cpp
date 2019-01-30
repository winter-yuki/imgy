#include "include/render/figures.hpp"

#include <cmath>
#include <exception>
#include <limits>
#include <memory>
#include <queue>


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
        return { NO_INTERSECT(), NO_NORMAL(), BLACK() };
    }

    Double sdiskr = std::sqrt(diskr);
    Double t1 = -k2 + sdiskr;
    Double t2 = -k2 - sdiskr;

    auto t = std::min(t1, t2);
    return { t, normal(ray.count(t)), color_ };
}


Vector FigSphere::normal(Vector const & point) const
{
    return (point - center_).normalize();
}


FigBox::FigBox(Vector const & b1, Vector const & b2, Color color)
    : color_(color)
{
    // TODO(args issue)
    //    if ((b1[0] < b2[0] && b1[1] < b2[1] && b1[2] < b2[2]) == false) {
    //        throw std::runtime_error("Coordinates of first point of box should be"
    //                                 "less than coordinates of second one");
    //    }

    faces_near_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 9), b1, color));
    faces_near_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b1, color));
    faces_near_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b1, color));

    faces_far_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 0), b2, color));
    faces_far_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b2, color));
    faces_far_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b2, color));
}


FigBox::Intersect FigBox::intersect(Ray const & ray) const
{
    // TODO()
    return { NO_INTERSECT(), NO_NORMAL(), BLACK() };
}


FigPlane::FigPlane(Vector const & n, Vector const & p, Color color)
    : n_    (n)
    , p_    (p)
    , d_    (-(n_ * p_))
    , color_(color)
{
    // Direct normal to cam
    if (count({0, 0, 0}) < -EPSILON()) { // TODO(cam_pos)
        n_ = n_ * -1;
    }
}


FigPlane::Intersect FigPlane::intersect(Ray const & ray) const
{
    auto r = ray.get_view();
    Double den = n_ * r.D;
    if (std::abs(den) < EPSILON()) {
        return { NO_INTERSECT(), NO_NORMAL(), BLACK() };
    }

    Double t = -(n_ * r.O + d_) / den;
    return { t, n_, color_ };
}


Double FigPlane::count(Vector const & point)
{
    return n_ * point + d_;
}

} // namespace Render



























