#include "include/render/light.hpp"


namespace Render
{

LightAmbient::LightAmbient(Double intensity)
    : intensity_(intensity)
{}


LightAmbient::Double LightAmbient::light(Vector const & /*normal*/,
                                         Vector const & /*point*/) const
{
    return intensity_;
}


LightPoint::LightPoint(Vector const & position, Double intensity)
    : pos_      (position)
    , intensity_(intensity)
{}


LightPoint::Double LightPoint::light(Vector const & normal,
                                     Vector const & point) const
{
    Vector L = pos_ - point;
    Double coef = (normal * L) / (normal.norm() * L.norm());
    if (coef <= 0) {
        return 0;
    }
    coef = 1; // TODO()
    return intensity_ * coef;
}


LightDirectional::LightDirectional(Vector const & direction, Double intensity)
    : dir_      (direction)
    , intensity_(intensity)
{}


LightDirectional::Double
LightDirectional::light(Vector const & normal, Vector const & /*point*/) const
{
    Double coef = (normal * dir_) / (normal.norm() * dir_.norm());
    if (coef <= 0) {
        return 0;
    }
    return intensity_ * coef;
}

} // namespace Render
