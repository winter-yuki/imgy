#include "include/render/noise.hpp"


namespace Render
{

Double PerlinNoise::operator()(Vector const & p) const
{
    // TODO(PerlinNoise::operator())
    return std::sin(p[0] * 100);
}

} // namespace Render
