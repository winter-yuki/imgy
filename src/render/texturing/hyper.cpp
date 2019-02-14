#include <utility>

#include "include/render/textures.hpp"


namespace Render
{

HyperTexture::HyperTexture(Color c1, Color c2, Double p, Noise noise)
    : c1_   (c1)
    , c2_   (c2)
    , p_    (p)
    , noise_(std::move(noise))
{}


Color HyperTexture::operator()(Vector const & point) const
{
    return c1_ + (c1_ - c2_) *
            func(std::sqrt(point[0] * point[0] + point[1] * point[1]), point);
}


Color HyperTexture::operator()(Double u, Double v) const
{

}

} // namespace Render
