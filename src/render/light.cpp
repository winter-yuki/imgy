#include "include/render/light.hpp"

#include <cassert>
#include <utility>


namespace Render
{

LightAmbient::LightAmbient(Double intensity)
    : intensity_(intensity)
{}


RPs LightAmbient::rays_to(Vector /*from*/) const
{
    return { std::make_pair(NULL_RAY(), EPSILON()) };
}


Double LightAmbient::light(Vector const & /*normal*/,
                           RayPos const & /*rp*/,
                           Vector const & /*cam_pos*/) const
{
    return intensity_;
}


LightPoint::LightPoint(Vector const & position, Double intensity)
    : pos_      (position)
    , intensity_(intensity)
{}


RPs LightPoint::rays_to(Vector from) const
{
    Ray r(pos_ - from, from);
    return { std::make_pair(r, r.count(pos_)) };
}

Double LightPoint::light(Vector const & normal,
                         RayPos const & rp,
                         Vector const & cam_pos) const
{
    assert(normal.is_normalized(EPSILON()));

    Double rez = 0;

    // Diffuse
    Vector L = (pos_ - rp.first.from()).normalized();
    Double coef = normal * L;
    if (coef < 0) {
        return 0;
    }
    rez += intensity_ * coef;

    // Specular
    const Double SPEC_COEF = 50;
    Vector cam_pos_point = (rp.first.from() - cam_pos).normalized();
    Vector R = cam_pos_point - normal * 2 * (cam_pos_point * normal);
    R.normalize();
    rez += intensity_ * std::pow(R * L, SPEC_COEF);

    return rez;
}


LightDirectional::LightDirectional(Vector const & direction, Double intensity)
    : dir_      (direction.normalized())
    , intensity_(intensity)
{}


RPs LightDirectional::rays_to(Vector from) const
{
    return { std::make_pair(Ray(dir_, from), INF_PARAM()) };
}


Double LightDirectional::light(Vector const & normal,
                               RayPos const & rp,
                               Vector const & cam_pos) const
{
    assert(normal.is_normalized(EPSILON()));
    assert(dir_.is_normalized(EPSILON()));

    Double rez = 0;

    // Diffuse
    // TODO(operator-)
    Double coef = normal * -dir_;
    if (coef <= 0) {
        return 0;
    }
    rez += intensity_ * coef;

    // Specular TODO(specular)
    const Double SPEC_COEF = 50;
    Vector cam_pos_point = (rp.first.from() - cam_pos).normalized();
    Vector R = cam_pos_point - normal * 2 * (cam_pos_point * normal);
    R.normalize();
    rez += intensity_ * std::pow(R * -dir_, SPEC_COEF);

    return rez;
}


LightSpheric::LightSpheric(Vector const & center,
                           Double radius, Double intensity)
    : center_   (center)
    , radius_   (radius)
    , intensity_(intensity / accuracy_) // TODO()
{}


RPs LightSpheric::rays_to(Vector from) const
{
    // TODO(LightSpheric::rays_to)
    Vector dir0  = center_ - from;
    Vector base1 = dir0.cross(Vector{1, 1, 1}).normalize();
    Vector base2 = dir0.cross(base1).normalize();

    SizeT n_rays = std::floor(accuracy_ * radius_);
    Double delta = radius_ / n_rays;
    Double coef  = 0;

    RPs rps;
    for (SizeT i = 0; i < n_rays; ++i) {
        Vector d1 = dir0 + base1 * coef;
        Vector d2 = dir0 - base1 * coef;
        Vector d3 = dir0 + base2 * coef;
        Vector d4 = dir0 - base2 * coef;

        Ray r1(d1, from);
        Ray r2(d2, from);
        Ray r3(d3, from);
        Ray r4(d4, from);

        // Supposing that radius is rather small.
        Double t1 = r1.count(d1);
        Double t2 = r1.count(d1);
        Double t3 = r1.count(d1);
        Double t4 = r1.count(d1);

        auto p1 = std::make_pair(r1, t1);
        auto p2 = std::make_pair(r2, t2);
        auto p3 = std::make_pair(r3, t3);
        auto p4 = std::make_pair(r4, t4);

        rps.push_back(p1);
        rps.push_back(p2);
        rps.push_back(p3);
        rps.push_back(p4);

        coef += delta;
    }

    return rps;
}


Double LightSpheric::light(Vector const & normal,
                           RayPos const & rp,
                           Vector const & /*cam_pos*/) const
{
    // TODO(LightSpheric::light)
    Vector to_center = (center_ - rp.first.from()).normalize();
    assert(rp.first.dir().is_normalized(EPSILON()));
    Double k1 = rp.first.dir() * to_center;
    if (k1 < 0) {
        return 0;
    }

    assert(normal.is_normalized(EPSILON()));
    Double k2 = normal * rp.first.dir();
    if (k2 < 0) {
        return 0;
    }

    return intensity_ * k1 * k2;
}

} // namespace Render
