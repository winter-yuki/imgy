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
                           RayPos const & /*view*/,
                           RayPos const & /*to_light*/) const
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
                         RayPos const & view,
                         RayPos const & to_light) const
{
    assert(normal.is_normalized(EPSILON()));

    Double rez = 0;

    // Diffuse
    Double coef = normal * to_light.first.dir();
    if (coef < 0) {
        return 0;
    }
    rez += intensity_ * coef;

    // Specular
    const Double SPEC_COEF = 50;
    auto refl = view.first.reflected(normal, view.second);
    rez += intensity_ * std::pow(refl.dir() * to_light.first.dir(), SPEC_COEF);

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
                               RayPos const & view,
                               RayPos const & /*to_light*/) const
{
    assert(normal.is_normalized(EPSILON()));
    assert(dir_.is_normalized(EPSILON()));

    Double rez = 0;

    // Diffuse
    Double coef = normal * -dir_;
    if (coef <= 0) {
        return 0;
    }
    rez += intensity_ * coef;

    // Specular
    const Double SPEC_COEF = 50;
    auto refl = view.first.reflected(normal, view.second);
    rez += intensity_ * std::pow(refl.dir() * -dir_, SPEC_COEF);

    return rez;
}


LightSpheric::LightSpheric(Vector const & center,
                           Double radius, Double intensity)
    : center_   (center)
    , radius_   (radius)
    , intensity_(intensity / accuracy_)
{}

/*
RPs LightSpheric::rays_to(Vector from) const
{
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
        Double t2 = r1.count(d2);
        Double t3 = r1.count(d3);
        Double t4 = r1.count(d4);

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
/**/

RPs LightSpheric::rays_to(Vector from) const
{
    Vector dir0  = center_ - from;
    Double t0    = Ray(dir0, from).count(center_);
    Vector base1 = dir0.cross(Vector{1, 1, 1}).normalize();
    Vector base2 = dir0.cross(base1).normalize();

    SizeT  n_circles   = std::floor(radius_ * accuracy_);
    Double delta_c     = radius_ / n_circles;
    Double curr_radius = 0;

    auto rays_quarter = [](SizeT circle) ->SizeT {
        return circle + 1;
    };

    RPs rps;
    for (SizeT circle = 0; circle < n_circles;
         ++circle, curr_radius += delta_c) {
        const auto rays     = 4 * rays_quarter(circle);
        const Double t_max  = M_PI;
        const Double t_step = t_max / rays;
        Double t = 0;
        for (SizeT i = 0; i < rays; ++i, t += t_step) {
            Vector d = dir0 + base1 * std::cos(t) + base2 * std::sin(t);\
            rps.push_back(std::make_pair(Ray{d, from}, t0));
        }
    }
    return rps;
}


Double LightSpheric::light(Vector const & normal,
                           RayPos const & /*view*/,
                           RayPos const & to_light) const
{
    Vector to_center = (center_ - to_light.first.from()).normalize();
    assert(to_light.first.dir().is_normalized(EPSILON()));
    Double k1 = to_light.first.dir() * to_center;
    if (k1 < 0) {
        return 0;
    }

    assert(normal.is_normalized(EPSILON()));
    Double k2 = normal * to_light.first.dir();
    if (k2 < 0) {
        return 0;
    }

    return intensity_ * k1 * k2;
}

} // namespace Render
