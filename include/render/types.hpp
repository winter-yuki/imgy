#ifndef INCLUDE_RENDER_TYPES_HPP
#define INCLUDE_RENDER_TYPES_HPP

#include <limits>

#include "include/math/glmath.hpp"
#include "include/file/types.hpp"


namespace Render
{

class RenderTypes {
public:
    using Double = double;
    using SizeT  = ImgTypes::SizeT;
    using Vector = GLMath::Vector<Double>;
    using Matrix = GLMath::Matrix<Double>;

    const Double EPSILON = 1e-7; // TODO(static)
    const Double NO_INTERSECT = std::numeric_limits<Double>::max();
    const Vector NO_NORMAL    = { 0, 0, 0 };
    const Vector CAM_POS      = { 0, 0, 0 };
};

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
