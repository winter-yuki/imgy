#include "include/render/objs.hpp"

#include <utility>

namespace Render
{

Sphere::Sphere(Point const & center, ValT radius)
    : center_(center)
    , radius_(radius)
{}

} // namespace Render
