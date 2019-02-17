#ifndef INCLUDE_RENDER_TEXTURES_HPP
#define INCLUDE_RENDER_TEXTURES_HPP

#include <functional>

#include "include/render/noise.hpp"
#include "include/render/types.hpp"
#include "include/utility/color.hpp"


namespace Render
{

class IMapping {
public:
    IMapping()                             = default;
    virtual ~IMapping()                    = default;
    IMapping(IMapping const &)             = delete;
    IMapping & operator=(IMapping const &) = delete;

    virtual Color operator()(Vector const & point) const = 0;
    virtual Color operator()(Double u, Double v)   const = 0;
};


class HyperTexture final
        : public IMapping {
public:
    using Noise = std::function<Double (Vector v)>;
    HyperTexture(Color const & c1, Color const & c2,
                 Double p, Noise noise);

    Color operator()(Vector const & point) const override;
    Color operator()(Double u, Double v)   const override;

private:
    Double func(Double a, Vector const & p) const;

    Color  c1_, c2_;
    Double p_;
    Noise  noise_;
};


class FigColor final
        : public IMapping {
public:
    explicit FigColor(Color const & c);

    Color operator()(Vector const & /*point*/)   const override;
    Color operator()(Double /*u*/, Double /*v*/) const override;

private:
    Color c_;
};

} // namespace Render


#endif // INCLUDE_RENDER_TEXTURES_HPP
