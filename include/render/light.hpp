#ifndef INCLUDE_RENDER_LIGHT_HPP
#define INCLUDE_RENDER_LIGHT_HPP

#include "include/render/objsbase.hpp"


namespace Render
{

class LightAmbient final
        : public ISceneLight {
public:
    explicit LightAmbient(Double intensity);

    RPs    rays_to  (Vector /*from*/) const override;
    Double light    (Vector const & /*normal*/,
                     RayPos const & /*rp*/,
                     Vector const & /*cam_pos*/) const override;

private:
    Double intensity_;
};


class LightPoint final
        : public ISceneLight {
public:
    LightPoint(Vector const & position, Double intensity);

    RPs    rays_to (Vector from) const override;
    Double light   (Vector const & normal,
                    RayPos const & rp,
                    Vector const & cam_pos) const override;

private:
    Vector pos_;
    Double intensity_;
};


class LightDirectional final
        : public ISceneLight {
public:
    LightDirectional(Vector const & direction, Double intensity);

    RPs    rays_to (Vector from) const override;
    Double light   (Vector const & normal,
                    RayPos const & rp,
                    Vector const & cam_pos) const override;

private:
    Vector dir_;
    Double intensity_;
};


class LightSpheric final
        : public ISceneLight {
public:
    LightSpheric(Vector const & center, Double radius, Double intensity);

    RPs    rays_to (Vector from) const override;
    Double light   (Vector const & normal,
                    RayPos const & rp,
                    Vector const & cam_pos) const override;

private:
    Vector center_;
    Double radius_;
    Double accuracy_ = 3; // Rays/radius
    Double intensity_;
};

} // namespace Render


#endif // INCLUDE_RENDER_LIGHT_HPP
