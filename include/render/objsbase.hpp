#ifndef INCLUDE_RENDER_OBJSBASE_HPP
#define INCLUDE_RENDER_OBJSBASE_HPP

#include "include/render/types.hpp"
#include "include/file/types.hpp"
#include "include/render/ray.hpp"


namespace Render
{

class ISceneObj
        : public ImgTypes
        , public RenderTypes {
public:
    ISceneObj()                              = default;
    virtual ~ISceneObj()                     = default;
    ISceneObj(ISceneObj const &)             = delete;
    ISceneObj & operator=(ISceneObj const &) = delete;
};


class ISceneFig
        : public ISceneObj {
public:
    using Intersect = std::pair<Double /*t*/, Color>;

public:
    virtual Intersect intersect(Ray const & ray) const = 0;
};


class ISceneLight
        : public ISceneObj {
public:
    // TODO(lights)
};

} // namespace Render


#endif // INCLUDE_RENDER_OBJSBASE_HPP
