#ifndef INCLUDE_RENDER_FIGURES_HPP
#define INCLUDE_RENDER_FIGURES_HPP

#include "include/render/objsbase.hpp"


namespace Render
{

class FigSphere final
        : public ISceneFig {
public:
    FigSphere(Vector const & center, Double radius, Color color);

    Intersect intersect(Ray const & ray) const override;

private:
    Vector center_;
    Double radius_;
    Color  color_;
};


class FigBox final
        : public ISceneFig {
public:
    Intersect intersect(Ray const & ray) const override;
};


class FigPlane final
        : public ISceneFig {
public:
    FigPlane(Vector const & n, Vector const & p, Color color);

    Intersect intersect(Ray const & ray) const override;

private:
    Vector n_; // {a, b, c}
    Vector p_;
    Double d_; // ax + by + cz + d = 0
    Color  color_;
};

}  // namespace Render


#endif // INCLUDE_RENDER_FIGURES_HPP
