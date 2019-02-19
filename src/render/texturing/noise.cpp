#include "include/render/noise.hpp"

#include <algorithm>
#include <numeric>
#include <random>

#include <boost/bind.hpp>


namespace Render
{

PerlinNoise::PerlinNoise()
    : perm_(std::vector<SizeT> (size_))
    , rnd_ (std::vector<Double>(size_))
{
    std::iota(perm_.begin(), perm_.end(), 0);
    std::random_device rd1;
    std::mt19937 g1(rd1());
    std::shuffle(perm_.begin(), perm_.end(), g1);

    std::random_device rd2;
    std::mt19937 g2(rd2());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::generate(rnd_.begin(), rnd_.end(), boost::bind(dis, g2));
}


Double PerlinNoise::operator()(Vector const & p) const
{
    auto v = p.view();
    return interp(range(v.x), range(v.y), range(v.z));
}


SizeT PerlinNoise::index(SizeT x, SizeT y, SizeT z) const
{
    auto index1 = [this](SizeT x) { return perm_[x % size_]; };
    auto index2 = [index1, this](SizeT x, SizeT y) {
        return index1(x + index1(y)); };
    return index2(x, index2(y, z)) % size_;
}


std::tuple<SizeT, SizeT, Double> PerlinNoise::range(Double x) const
{
    SizeT x1 = std::floor(x);
    Double fx = x - x1;
    fx = (3 - 2 * fx) * fx * fx;
    x1 %= size_;
    SizeT x2 = (x1 + 1) % size_;
    return { x1, x2, fx };
}


Double PerlinNoise::interp(Rng const & x, Rng const & y, Rng const & z) const
{
    SizeT  x1{}, x2{};
    SizeT  y1{}, y2{};
    SizeT  z1{}, z2{};
    Double fx{}, fy{}, fz{};

    using std::tie;
    tie(x1, x2, fx) = x;
    tie(y1, y2, fy) = y;
    tie(z1, z2, fz) = z;

    Double x00 = rnd_[index(x1, y1, z1)] * (1 - fx) + rnd_[index(x2, y1, z1)] * fx;
    Double x01 = rnd_[index(x1, y2, z1)] * (1 - fx) + rnd_[index(x2, y2, z1)] * fx;
    Double x10 = rnd_[index(x1, y1, z2)] * (1 - fx) + rnd_[index(x2, y1, z2)] * fx;
    Double x11 = rnd_[index(x1, y2, z2)] * (1 - fx) + rnd_[index(x2, y2, z2)] * fx;

    Double y00 = x00 * (1 - fy) + x01;
    Double y01 = x10 * (1 - fy) + x11;

    Double z0 = y00 * (1 - fz) + y01;

    return z0;
}

} // namespace Render



























