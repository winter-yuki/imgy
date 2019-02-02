#include "include/render/light.hpp"

#include <cassert>


namespace Render
{

LightAmbient::LightAmbient(Double intensity)
    : intensity_(intensity)
{}


Vector LightAmbient::dir_to(Vector /*from*/) const
{
    return NULL_VECTOR();
}


Double LightAmbient::pos_param(Ray const & /*ray*/) const
{
    return EPSILON();
}


Double LightAmbient::light(Vector const & /*normal*/,
                           Vector const & /*point*/,
                           Vector const & /*cam_pos*/) const
{
    return intensity_;
}


LightPoint::LightPoint(Vector const & position, Double intensity)
    : pos_      (position)
    , intensity_(intensity)
{}


Vector LightPoint::dir_to(Vector from) const
{
    return (pos_ - from).normalize();
}


Double LightPoint::pos_param(Ray const & ray) const
{
    return ray.count(pos_);
}


Double LightPoint::light(Vector const & normal,
                         Vector const & point,
                         Vector const & cam_pos) const
{
    assert(normal.is_normalized(EPSILON()));

    Double rez = 0;

    // Diffuse
    Vector L = (pos_ - point).normalized();
    Double coef = normal * L;
    if (coef < 0) {
        return 0;
    }
    rez += intensity_ * coef;

    // Specular
    const Double SPEC_COEF = 50;
    Vector cam_pos_point = (point - cam_pos).normalized();
    Vector R = cam_pos_point - normal * 2 * (cam_pos_point * normal);
    R.normalize();
    rez += intensity_ * std::pow(R * L, SPEC_COEF);

    return rez;
}


LightDirectional::LightDirectional(Vector const & direction, Double intensity)
    : dir_      (direction.normalized())
    , intensity_(intensity)
{}


Vector LightDirectional::dir_to(Vector /*from*/) const
{
    return dir_;
}


Double LightDirectional::pos_param(Ray const & /*ray*/) const
{
    return INF_PARAM();
}


Double LightDirectional::light(Vector const & normal,
                               Vector const & point,
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

    // Specular
    const Double SPEC_COEF = 50;
    Vector cam_pos_point = (point - cam_pos).normalized();
    Vector R = cam_pos_point - normal * 2 * (cam_pos_point * normal);
    R.normalize();
    rez += intensity_ * std::pow(R * (0 - dir_), SPEC_COEF);

    return rez;
}

} // namespace Render
