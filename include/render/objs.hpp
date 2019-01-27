#ifndef INCLUDE_RENDER_OBJS_HPP
#define INCLUDE_RENDER_OBJS_HPP

#include <utility>

#include "include/file/bmp.hpp"
#include "include/render/types.hpp"


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
    using Intersect = std::pair<ValT, Color>;

public:
    virtual Intersect intersect(Vector const & ray,
                                Vector const & cam_pos) = 0;
};


class Sphere final
        : public ISceneFig {
public:
    Sphere(Vector const & center, ValT radius, Color color);

    Intersect intersect(Vector const & ray, Vector const & cam_pos) override;


private:
    Vector center_;
    ValT   radius_;
    Color  color_;
};
// TODO(obj types)

class ISceneLight
        : public ISceneObj {
public:
    // TODO(lights)
};

}  // namespace Render


#endif // INCLUDE_RENDER_OBJS_HPP
