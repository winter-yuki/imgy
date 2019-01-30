#ifndef INCLUDE_RENDER_RENDER_HPP
#define INCLUDE_RENDER_RENDER_HPP

#include <vector>

#include "include/file/filebase.hpp"
#include "include/render/objsbase.hpp"
#include "include/render/ray.hpp"
#include "include/render/types.hpp"
#include "include/utility/color.hpp"


namespace Render
{

class Render final {
public:
    using Figures = std::vector<ISceneFig *>;
    using Lights  = std::vector<ISceneLight *>;

public:
    Render(IImgFile & image, Figures objs, Lights lts);
    Render(Render const &)             = delete;
    Render & operator=(Render const &) = delete;
    Render(Render &&)                  = delete;
    Render & operator=(Render &&)      = delete;

    void render();


private:
    void   prep_dirs   ();
    Ray    calc_ray_dir(SizeT row, SizeT col)                        const;
    Color  trace_ray   (Ray const & ray)                             const;
    Double count_light (Vector const & point, Vector const & normal) const;

private:
    Color  background_color_ = BLACK();
    Double vport_w_ = 2;
    Double vport_h_ = 2;
    Double dist_    = 1;

    Vector up_    = {0, 1, 0}; // oy
    Vector to_    = {0, 0, 1}; // oz
    Vector pos_   = {0, 0, 0};
    Vector right_;

    IImgFile & image_;
    Figures    figs_;
    Lights     lts_;
};

}  // namespace Render


#endif // INCLUDE_RENDER_RENDER_HPP
