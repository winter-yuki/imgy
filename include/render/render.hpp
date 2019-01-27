#ifndef INCLUDE_RENDER_RENDER_HPP
#define INCLUDE_RENDER_RENDER_HPP

#include <vector>

#include "include/file/imgfile.hpp"
#include "include/render/objs.hpp"
#include "include/render/types.hpp"


namespace Render
{

class Render final
        : public ImgTypes
        , public RenderTypes {
public:
    using Objects = std::vector<ISceneFig *>;

public:
    Render(IImgFile & image, Objects objs);
    Render(Render const &)             = delete;
    Render & operator=(Render const &) = delete;
    Render(Render &&)                  = delete;
    Render & operator=(Render &&)      = delete;

    void render();


private:
    Vector calc_ray_dir(SizeT row, SizeT col);
    Color  trace_ray   (Vector const & ray);

private: // TODO(ray separate class)
    ValT scr_w_ = 50;
    ValT scr_h_ = 50;
    ValT dist_  = 50;

    Vector cam_pos_;
    Vector cam_up_ = {0, 1, 0}; // oy
    Vector cam_to_ = {0, 0, 1}; // oz

    Vector pos_scr_;
    Vector right_;

    IImgFile & image_;
    Objects    objs_;
};

}  // namespace Render

#endif // INCLUDE_RENDER_RENDER_HPP
