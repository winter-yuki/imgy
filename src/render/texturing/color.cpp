#include "include/render/textures.hpp"


namespace Render
{

FigColor::FigColor(Color const & c)
    : c_(c)
{}


Color FigColor::operator()(Vector const & /*point*/) const
{
    return c_;
}


Color FigColor::operator()(Double /*u*/, Double /*v*/) const
{
    return c_;
}

} // namespace Render
