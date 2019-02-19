#ifndef INCLUDE_RENDER_NOISE_HPP
#define INCLUDE_RENDER_NOISE_HPP

#include <tuple>
#include <vector>

#include "include/render/types.hpp"


namespace Render
{

class PerlinNoise final {
public:
    PerlinNoise();

    Double operator()(Vector const & p) const;

private:
    using Rng = std::tuple<SizeT, SizeT, Double>;
    SizeT  index(SizeT x, SizeT y, SizeT z) const;
    Rng    range(Double x)                  const;
    Double interp(Rng const & x, Rng const & y, Rng const & z) const;

    const SizeT size_ = 100;
    std::vector<SizeT>  perm_;
    std::vector<Double> rnd_;
};

} // namespace Render


#endif // INCLUDE_RENDER_NOISE_HPP
