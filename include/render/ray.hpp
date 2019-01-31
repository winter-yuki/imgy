#ifndef INCLUDE_RENDER_RAY_HPP
#define INCLUDE_RENDER_RAY_HPP

#include <cmath>
#include <utility>

#include "include/render/types.hpp"


namespace Render
{

class Ray final {
public:
    struct View {
        Vector const & D; // Direction
        Vector const & O; // Point of begining
    };

public:
    Ray(Vector const & dir, Vector const & from)
        : dir_ (dir)
        , from_(from)
    { dir_.normalize(); dir_[3] = 0; }

    Vector count(Double t) const { return from_ + dir_ * t; }
    Double count(Vector const & point) const {
        Vector to = point - from_;
        Double t0 = to[0] / dir_[0];
        Double t1 = to[1] / dir_[1];
        Double t2 = to[2] / dir_[2];
        if (std::abs(t0 - t1) < EPSILON() && std::abs(t1 - t2) < EPSILON()) {
            return t0;
        }
        return INF_PARAM();
    }

    View get_view() const { return { dir_, from_ }; }

    Vector dir () const { return dir_;  }
    Vector from() const { return from_; }

private:
    Vector dir_;
    Vector from_;
};

} // namespace Render


#endif // INCLUDE_RENDER_RAY_HPP
