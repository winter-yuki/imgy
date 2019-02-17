#ifndef INCLUDE_RENDER_NOISE_HPP
#define INCLUDE_RENDER_NOISE_HPP

#include "include/render/types.hpp"


namespace Render
{

class PerlinNoise final {
public:
    Double operator()(Vector const & p) const;

private:
};

} // namespace Render


#endif // INCLUDE_RENDER_NOISE_HPP
