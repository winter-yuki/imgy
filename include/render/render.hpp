#ifndef INCLUDE_RENDER_RENDER_HPP
#define INCLUDE_RENDER_RENDER_HPP

#include "include/math/glmath.hpp"
#include "include/file/imgfile.hpp"
#include "include/render/types.hpp"


namespace Render
{

class Render final
        : public IImgTypes {
public:
    using Vector = GLMath::Vector<ValT>;
    using Matrix = GLMath::Matrix<ValT>;

public:

    Render();
};

}  // namespace Render

#endif // INCLUDE_RENDER_RENDER_HPP
