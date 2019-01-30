#include "include/render/light.hpp"


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
                           Vector const & /*point*/) const
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
                         Vector const & point) const
{
    Vector L = pos_ - point;
    Double coef = (normal * L) / (normal.norm() * L.norm());
    if (coef < 0) {
        return 0;
    }
    return intensity_ * coef;
}


LightDirectional::LightDirectional(Vector const & direction, Double intensity)
    : dir_      (direction)
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
                               Vector const & /*point*/) const
{
    Double coef = (normal * dir_) / (normal.norm() * dir_.norm());
    if (coef <= 0) {
        return 0;
    }
    return intensity_ * coef;
}

} // namespace Render
