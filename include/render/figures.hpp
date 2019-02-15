#ifndef INCLUDE_RENDER_FIGURES_HPP
#define INCLUDE_RENDER_FIGURES_HPP

#include <memory>
#include <vector>

#include "include/render/objsbase.hpp"
#include "include/render/textures.hpp"


namespace Render
{

class FigPlane final
        : public ISceneFig {
public:
    FigPlane(Vector const & n, Vector const & p, Color const & color,
             Vector const & cam_pos, Material m = Material());

    Intersect intersect  (Ray const & ray) const  override;
    void      set_cam_pos(Vector const & new_pos) override;

private:
    Double count(Vector const & point);

    Vector   n_; // {a, b, c}
    Vector   p_;
    Double   d_; // ax + by + cz + d_ = 0
    Color    color_;
    Material m_;
};


class FigSphere final
        : public ISceneFig {
public:
    FigSphere(Vector const & center, Double radius,
              IMapping const & map, Material m = Material());

    Intersect intersect  (Ray const & ray)      const override;
    void      set_cam_pos(Vector const & /*new_pos*/) override;

private:
    Vector normal(Vector const & point) const;
    Vector shift_map(Vector const & p) const;

    Vector   center_;
    Double   radius_;
    Material m_;
    IMapping const & map_;
};


class FigBox final
        : public ISceneFig {
public:
    FigBox(Vector const & b1, Vector const & b2, Color color,
           Vector const & cam_pos, Material m = Material());

    Intersect intersect  (Ray const & ray) const  override;
    void      set_cam_pos(Vector const & new_pos) override;

private:
    Vector b1_, b2_;
    std::vector<std::shared_ptr<FigPlane>> faces_;
    Color    color_;
    Material m_;
};

}  // namespace Render


#endif // INCLUDE_RENDER_FIGURES_HPP
