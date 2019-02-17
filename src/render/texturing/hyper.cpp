#include <utility>

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
    auto p = point.normalized().view();
    auto k = func(std::sqrt(p.x * p.x + p.y * p.y) * 100 /* TODO() */, point);
    return c1_ + k * (c2_ - c1_);
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
