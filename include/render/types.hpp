#ifndef INCLUDE_RENDER_TYPES_HPP
#define INCLUDE_RENDER_TYPES_HPP

#include <limits>
#include <tuple>

#include "include/math/glmath.hpp"
#include "include/utility/color.hpp"


namespace Render
{

using Double    = double;
using SizeT     = int64_t;
using Vector    = GLMath::Vector<Double>;
using Matrix    = GLMath::Matrix<Double>;


inline constexpr Double EPSILON()   { return 1e-5; }
inline constexpr Double INF_PARAM() {
    return std::numeric_limits<Double>::max(); }
inline Vector NULL_VECTOR() { return Vector{0, 0, 0, 0}; }


struct Material final {
    Material() = default;
    explicit Material(Double fong) : FONG(fong) {}

    Double FONG = 50;
};

struct Intersect final {
    Intersect() = default;
    explicit Intersect(Color const & bg) : c(bg) {}
    Intersect(Double t, Vector const & n, Color const & c, Material m)
        : t(t), n(n), c(c), m(m) {}

    Double   t = INF_PARAM();
    Vector   n = NULL_VECTOR();
    Color    c = BLACK();
    Material m;
};


inline Intersect NO_INTERSECT() { return {}; }

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
