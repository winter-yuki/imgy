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
using Intersect = std::tuple<Double /*t*/, Vector /*normal*/, Color>;

inline constexpr Double EPSILON()   { return 1e-7; }
inline constexpr Double INF_PARAM() {
    return std::numeric_limits<Double>::max(); }
inline Vector NULL_VECTOR() { return Vector{0, 0, 0, 0}; }

inline Intersect NO_INTERSECT() { return { INF_PARAM(), NULL_VECTOR(), {} }; }

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
