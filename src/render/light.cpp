#include "include/render/light.hpp"


namespace Render
{

LightAmbient::LightAmbient(Double intensity)
    : intensity_(intensity)
{}


Double LightAmbient::light(Vector const & /*normal*/,
                           Vector const & /*point*/) const
{
    return intensity_;
}


LightPoint::LightPoint(Vector const & position, Double intensity)
    : pos_      (position)
    , intensity_(intensity)
{}


Double LightPoint::light(Vector const & normal,
                         Vector const & point) const
{
    Vector L = pos_ - point;
    //    Vector L = point - pos_; // TODO()
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
