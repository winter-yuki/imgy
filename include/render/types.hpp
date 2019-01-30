#ifndef INCLUDE_RENDER_TYPES_HPP
#define INCLUDE_RENDER_TYPES_HPP

#include <limits>

#include "include/math/glmath.hpp"


namespace Render
{

using Double = double;
using SizeT  = int64_t;
using Vector = GLMath::Vector<Double>;
using Matrix = GLMath::Matrix<Double>;

inline constexpr Double EPSILON()      { return 1e-7; }
inline constexpr Double NO_INTERSECT() {
    return std::numeric_limits<Double>::max(); }
inline           Vector NO_NORMAL()    { return Vector{0, 0, 0, 0}; }

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
