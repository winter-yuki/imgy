#include "include/render/figures.hpp"


namespace Render
{

FigSphere::FigSphere(Vector const & center, Double radius,
                     IMapping const & map, Material m)
    : center_(center)
    , radius_(radius)
    , m_     (m)
    , map_   (map)
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
    auto p = ray.count(t);
    return { t, normal(p), map_(shift_map(p)), m_ };
}


void FigSphere::set_cam_pos(Vector const & /*new_pos*/)
{}


Vector FigSphere::normal(Vector const & point) const
{
    return (point - center_).normalize();
}


Vector FigSphere::shift_map(Vector const & p) const
{
    return p - center_;
}

} // namespace Render
