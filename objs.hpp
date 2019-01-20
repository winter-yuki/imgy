#ifndef OBJS_HPP
#define OBJS_HPP


class ISceneObj {
public:
    using ValT = double;
    const ValT EPSILON = 1e-10;
    struct Point { ValT x, y, z; };

    ISceneObj()                              = default;
    virtual ~ISceneObj()                     = default;
    ISceneObj(ISceneObj const &)             = delete;
    ISceneObj & operator=(ISceneObj const &) = delete;
};


class Sphere final : ISceneObj {
public:
    Sphere(Point const & center, ValT radius);

private:
    Point center_;
    ValT  radius_;
};


#endif // OBJS_HPP
