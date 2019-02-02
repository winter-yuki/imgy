#ifndef INCLUDE_RENDER_LIGHT_HPP
#define INCLUDE_RENDER_LIGHT_HPP

#include "include/render/objsbase.hpp"


namespace Render
{

class LightAmbient final
        : public ISceneLight {
public:
    explicit LightAmbient(Double intensity);

    Vector dir_to   (Vector /*from*/)     const override;
    Double pos_param(Ray const & /*ray*/) const override;
    Double light    (Vector const & /*normal*/,
                     Vector const & /*point*/,
                     Vector const & /*cam_pos*/) const override;

private:
    Double intensity_;
};


class LightPoint final
        : public ISceneLight {
public:
    LightPoint(Vector const & position, Double intensity);

    Vector dir_to   (Vector from)     const override;
    Double pos_param(Ray const & ray) const override;
    Double light    (Vector const & normal,
                     Vector const & point,
                     Vector const & cam_pos) const override;

private:
    Vector pos_;
    Double intensity_;
};


class LightDirectional final
        : public ISceneLight {
public:
    LightDirectional(Vector const & direction, Double intensity);

    Vector dir_to   (Vector /*from*/)     const override;
    Double pos_param(Ray const & /*ray*/) const override;
    Double light    (Vector const & normal,
                     Vector const & point,
                     Vector const & cam_pos) const override;

private:
    Vector dir_;
    Double intensity_;
};

} // namespace Render


#endif // INCLUDE_RENDER_LIGHT_HPP
