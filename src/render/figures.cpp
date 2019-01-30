#include "include/render/figures.hpp"

#include <algorithm>
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


Intersect FigSphere::intersect(Ray const & ray) const
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


void FigSphere::set_cam_pos(Vector const & /*new_pos*/)
{}


Vector FigSphere::normal(Vector const & point) const
{
    return (point - center_).normalize();
}


FigBox::FigBox(Vector const & b1, Vector const & b2, Color color,
               Vector const & cam_pos)
    : color_(color)
{
    // WARNING (args issue)
    if ((b1[0] <= b2[0] && b1[1] <= b2[1] && b1[2] <= b2[2]) == false) {
        throw std::runtime_error("Coordinates of first point of box should be"
                                 "less than coordinates of second one");
    }

    faces_near_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 9), b1, color, cam_pos));
    faces_near_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b1, color, cam_pos));
    faces_near_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b1, color, cam_pos));

    faces_far_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 0), b2, color, cam_pos));
    faces_far_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b2, color, cam_pos));
    faces_far_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b2, color, cam_pos));
}


Intersect FigBox::intersect(Ray const & ray) const
{
    // TODO()
//    Intersect rez;

//    Double t_near_max = 0;
//    for (auto const & face : faces_near_) {
//        Double t{};
//        Vector n;
//        Color  c{};
//        Intersect inter   = face->intersect(ray);
//        std::tie(t, n, c) = inter;

//        if (t_near_max < t && std::abs(t - NO_INTERSECT()) > EPSILON()) {
//            t_near_max = t;
//            rez        = inter;
//        }
//    }

//    Double t_far_min  = INF_PARAM();
//    for (auto const & face : faces_far_) {
//        Double t{};
//        Vector n;
//        Color  c{};
//        std::tie(t, n, c) = face->intersect(ray);
//        t_far_min = std::min(t_far_min, t);
//    }

//    if (t_near_max < t_far_min) {
//        return rez;
//    }
    return { NO_INTERSECT(), NO_NORMAL(), BLACK() };
}


void FigBox::set_cam_pos(Vector const & new_pos)
{
    for (auto & face : faces_near_) {
        face->set_cam_pos(new_pos);
    }
    for (auto & face : faces_far_) {
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
        return { NO_INTERSECT(), NO_NORMAL(), BLACK() };
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



























