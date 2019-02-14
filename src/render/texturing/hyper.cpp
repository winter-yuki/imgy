#include <utility>

#include "include/render/textures.hpp"


namespace Render
{

HyperTexture::HyperTexture(Color const & c1, Color const & c2,
                           Double p, Noise noise)
    : c1_   (c1)
    , c2_   (c2)
    , p_    (p)
    , noise_(std::move(noise))
{}


Color HyperTexture::operator()(Vector const & point) const
{
    auto p = point.view();
    return c1_ + (c1_ - c2_) * func(std::sqrt(p.x * p.x + p.y * p.y), point);
}


Color HyperTexture::operator()(Double u, Double v) const
{
    return (*this)({u, v, 0});
}


Double HyperTexture::func(Double a, Vector const & p) const
{
    Double tmp = std::sin(a + noise_(p));
    return std::pow((1 + tmp) / 2, p_);
}

} // namespace Render
