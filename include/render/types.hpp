#ifndef INCLUDE_RENDER_TYPES_HPP
#define INCLUDE_RENDER_TYPES_HPP

#include "include/math/glmath.hpp"


namespace Render
{

class RenderTypes {
public:
    using Double = double;
    using Vector = GLMath::Vector<Double>;
    using Matrix = GLMath::Matrix<Double>;
    constexpr static const Double EPSILON = 1e-7;
};

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
