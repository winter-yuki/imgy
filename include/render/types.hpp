#ifndef INCLUDE_RENDER_TYPES_HPP
#define INCLUDE_RENDER_TYPES_HPP

#include "include/math/glmath.hpp"


namespace Render
{

class RenderTypes {
public:
    using ValT   = double;
    using Vector = GLMath::Vector<ValT>;
    using Matrix = GLMath::Matrix<ValT>;
    constexpr static const ValT EPSILON = 1e-7;
};

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
