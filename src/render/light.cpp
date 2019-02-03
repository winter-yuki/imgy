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


Double LightAmbient::pos_param(Ray const & /*ray*/) const
{
    return EPSILON();
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


Double LightPoint::pos_param(Ray const & ray) const
{
    return ray.count(pos_);
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


Double LightDirectional::pos_param(Ray const & /*ray*/) const
{
    return INF_PARAM();
}


Double LightDirectional::light(Vector const & normal,
                               RayPos const & rp,
                               Vector const & cam_pos) const
{
    assert(normal.is_normalized(EPSILON()));
    assert(dir_.is_normalized(EPSILON()));

    Double rez = 0;

    // Diffuse
    Double coef = normal * (0 - dir_);
    if (coef <= 0) {
        return 0;
    }
    rez += intensity_ * coef;

    // Specular TODO(specular)
    const Double SPEC_COEF = 50;
    Vector cam_pos_point = (rp.first.from() - cam_pos).normalized();
    Vector R = cam_pos_point - normal * 2 * (cam_pos_point * normal);
    R.normalize();
    rez += intensity_ * std::pow(R * (0 - dir_), SPEC_COEF);

    return rez;
}

} // namespace Render
