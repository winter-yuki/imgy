#ifndef INCLUDE_RENDER_FIGURES_HPP
#define INCLUDE_RENDER_FIGURES_HPP

#include <memory>
#include <vector>

#include "include/render/objsbase.hpp"


namespace Render
{

class FigPlane final
        : public ISceneFig {
public:
    FigPlane(Vector const & n, Vector const & p, Color color,
             Vector const & cam_pos);

    Intersect intersect  (Ray const & ray) const  override;
    void      set_cam_pos(Vector const & new_pos) override;

private:
    Double count(Vector const & point);

    Vector n_; // {a, b, c}
    Vector p_;
    Double d_; // ax + by + cz + d = 0
    Color  color_;
};


class FigSphere final
        : public ISceneFig {
public:
    FigSphere(Vector const & center, Double radius, Color color);

    Intersect intersect  (Ray const & ray)      const override;
    void      set_cam_pos(Vector const & /*new_pos*/) override;

private:
    Vector normal(Vector const & point) const;

    Vector center_;
    Double radius_;
    Color  color_;
};


class FigBox final
        : public ISceneFig {
public:
    FigBox(Vector const & b1, Vector const & b2, Color color,
           Vector const & cam_pos);

    Intersect intersect  (Ray const & ray) const  override;
    void      set_cam_pos(Vector const & new_pos) override;

private:
    Vector b1_, b2_;
    std::vector<std::shared_ptr<FigPlane>> faces_;
    Color color_;
};

}  // namespace Render


#endif // INCLUDE_RENDER_FIGURES_HPP
