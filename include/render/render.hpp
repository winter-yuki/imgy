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
    Render() = default;


private:
    ValT  scr_w_ = 500;
    ValT  scr_h_ = 500;
    SizeT pix_w_ = 500;
    SizeT pix_h_ = 500;

    ValT   dist_ = 10;
    Vector cam_pos_;
    Vector cam_up_;
    Vector cam_to_;
};

}  // namespace Render

#endif // INCLUDE_RENDER_RENDER_HPP
