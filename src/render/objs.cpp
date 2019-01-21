#include "include/render/objs.hpp"

#include <utility>

namespace Render
{

Sphere::Sphere(Vector const & center, ValT radius, Color color)
    : center_(center)
    , radius_(radius)
    , color_ (color)
{}


Sphere::Intersect Sphere::intersect(Vector const & ray, Vector const & cam_pos)
{
    // TODO(shere intersect)
}

} // namespace Render
