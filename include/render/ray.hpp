#ifndef INCLUDE_RENDER_RAY_HPP
#define INCLUDE_RENDER_RAY_HPP

#include <utility>

#include "include/render/types.hpp"


namespace Render
{

class Ray final
        : public RenderTypes
{
    // TODO(Ray)
public:
    Ray(Vector const & dir, Vector const & from)
        : dir_ (dir)
        , from_(from)
    {}


private:
    Vector dir_;
    Vector from_;
};

} // namespace Render


#endif // INCLUDE_RENDER_RAY_HPP
