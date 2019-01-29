#ifndef INCLUDE_RENDER_FIGURES_HPP
#define INCLUDE_RENDER_FIGURES_HPP

#include <memory>
#include <vector>

#include "include/render/color.hpp"
#include "include/render/objsbase.hpp"


namespace Render
{

class FigPlane final
        : public ISceneFig {
public:
    FigPlane(Vector const & n, Vector const & p, Color color);

    Intersect intersect(Ray const & ray) const override;

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

    Intersect intersect(Ray const & ray) const override;

private:
    Vector normal(Vector const & point) const;

    Vector center_;
    Double radius_;
    Color  color_;
};


class FigBox final
        : public ISceneFig {
public:
    FigBox(Vector const & b1, Vector const & b2, Color color);

    Intersect intersect(Ray const & ray) const override;

private:
    std::vector<std::shared_ptr<FigPlane>> faces_near_;
    std::vector<std::shared_ptr<FigPlane>> faces_far_;
    Color color_;
};

}  // namespace Render


#endif // INCLUDE_RENDER_FIGURES_HPP
