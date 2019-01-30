#ifndef INCLUDE_RENDER_OBJSBASE_HPP
#define INCLUDE_RENDER_OBJSBASE_HPP

#include "include/render/ray.hpp"
#include "include/render/types.hpp"
#include "include/utility/color.hpp"


namespace Render
{

class ISceneObj {
public:
    ISceneObj()                              = default;
    virtual ~ISceneObj()                     = default;
    ISceneObj(ISceneObj const &)             = delete;
    ISceneObj & operator=(ISceneObj const &) = delete;
};


class ISceneFig
        : public ISceneObj {
public:
    virtual Intersect intersect  (Ray const & ray) const  = 0;
    virtual void      set_cam_pos(Vector const & new_pos) = 0;
};


class ISceneLight
        : public ISceneObj {
public:
    virtual Double light(Vector const & /*normal*/,
                         Vector const & /*point*/) const = 0;
};

} // namespace Render


#endif // INCLUDE_RENDER_OBJSBASE_HPP
