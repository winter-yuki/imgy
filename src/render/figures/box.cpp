#include "include/render/figures.hpp"


namespace Render
{

FigBox::FigBox(Vector const & b1, Vector const & b2, Color color,
               Vector const & cam_pos, Material m)
    : b1_(b1 - EPSILON() /*to prevent round issues*/)
    , b2_(b2 + EPSILON())
    , color_(color)
    , m_(m)
{
    if ((b1[0] <= b2[0] && b1[1] <= b2[1] && b1[2] <= b2[2]) == false) {
        throw std::runtime_error("Coordinates of first point of box should be"
                                 "less than coordinates of second one");
    }

    faces_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 9), b1, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b1, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b1, color, cam_pos));

    faces_.push_back(std::make_shared<FigPlane>(Vector(1, 0, 0), b2, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 1, 0), b2, color, cam_pos));
    faces_.push_back(std::make_shared<FigPlane>(Vector(0, 0, 1), b2, color, cam_pos));
}


Intersect FigBox::intersect(Ray const & ray) const
{
    auto in_box = [this](Vector const & point) -> bool {
        return point >= b1_ && point <= b2_;
    };

    Double    t_min = INF_PARAM();
    Intersect inter = NO_INTERSECT();

    for (auto const & face : faces_) {
        auto i = face->intersect(ray);

        if (i.t < t_min && in_box(ray.count(i.t))) {
            t_min = i.t;
            inter = i;
        }
    }

    return inter;
}


void FigBox::set_cam_pos(Vector const & new_pos)
{
    for (auto & face : faces_) {
        face->set_cam_pos(new_pos);
    }
}

} // namespace Render
