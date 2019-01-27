#ifndef INCLUDE_RENDER_FIGURES_HPP
#define INCLUDE_RENDER_FIGURES_HPP

#include "include/render/objsbase.hpp"


namespace Render
{

class FigSphere final
        : public ISceneFig {
public:
    FigSphere(Vector const & center, Double radius, Color color);

    Intersect intersect(Vector const & ray, Vector const & cam_pos) override;

private:
    Vector center_;
    Double radius_;
    Color  color_;
};


class FigCuboid final
        : public ISceneFig {
public:
    Intersect intersect(Vector const & ray, Vector const & cam_pos) override;
};


class FigPlane final
        : public ISceneFig {
public:
    Intersect intersect(Vector const & ray, Vector const & cam_pos) override;
};

}  // namespace Render

#endif // INCLUDE_RENDER_FIGURES_HPP
