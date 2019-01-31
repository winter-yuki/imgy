#include "include/render/figures.hpp"

#include <algorithm>
#include <cmath>
#include <exception>
#include <functional>
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


Intersect FigSphere::intersect(Ray const & ray) const
{
    auto r = ray.get_view();

    Vector OC = r.O - center_;
    Double k1 = r.D * r.D;
    Double k2 = OC * r.D;
    Double k3 = OC * OC - radius_ * radius_;
    Double diskr = k2 * k2 - k1 * k3;
    if (diskr < 0) {
        return NO_INTERSECT();
    }

    Double sdiskr = std::sqrt(diskr);
    Double t1 = -k2 + sdiskr;
    Double t2 = -k2 - sdiskr;

    auto t = std::min(t1, t2);
    return { t, normal(ray.count(t)), color_ };
}


void FigSphere::set_cam_pos(Vector const & /*new_pos*/)
{}


Vector FigSphere::normal(Vector const & point) const
{
    return (point - center_).normalize();
}


FigBox::FigBox(Vector const & b1, Vector const & b2, Color color,
               Vector const & cam_pos)
    : b1_(b1 - EPSILON() /*to prevent round issues*/)
    , b2_(b2 + EPSILON())
    , color_(color)
{
    if ((b1[0] <= b2[0] && b1[1] <= b2[1] && b1[2] <= b2[2]) == false) {
        throw std::runtime_error("Coordinates of first point of box should be"
                                 "less than coordinates of second one");
    }

    faces_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 9), b1, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b1, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b1, color, cam_pos));

    faces_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 0), b2, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b2, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b2, color, cam_pos));
}


Intersect FigBox::intersect(Ray const & ray) const
{
    auto in_box = [this](Vector const & point) -> bool {
        return point >= b1_ && point <= b2_;
    };

    Double    t_min = INF_PARAM();
    Intersect inter = NO_INTERSECT();

    for (auto const & face : faces_) {
        Double t{};
        Vector n{};
        Color  c{};
        Intersect i = face->intersect(ray);
        std::tie(t, n, c) = i;

        if (t < t_min && in_box(ray.count(t))) {
            t_min = t;
            inter = i;
        }
    }

    return inter;
}


void FigBox::set_cam_pos(Vector const & new_pos)
{
    for (auto & face : faces_) {
        face->set_cam_pos(new_pos);
    }
}


FigPlane::FigPlane(Vector const & n, Vector const & p, Color color,
                   Vector const & cam_pos)
    : n_    (n)
    , p_    (p)
    , d_    (-(n_ * p_))
    , color_(color)
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
    return { t, n_, color_ };
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



























