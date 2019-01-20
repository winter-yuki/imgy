#include "include/imgproc/imgproc.hpp"

#include <memory>


void ImgEff::warp(Point from, Point dest)
{
    // Get back mapping.
    auto map = get_back_mapping(from, dest);

    std::shared_ptr<IImgFile> new_img(image_.clone());
    for (SizeT y = 0; y < rows(); ++y) {
        for (SizeT x = 0; x < cols(); ++x) {
            auto transform = find_triangle(map, {x, y});

            PointH ph;
            ph << x, y, 1;
            ph *= transform;
            auto p = convert(ph);

            if (p.x >= 0 && p.x < cols() &&
                    p.y >= 0 && p.y < rows()) {
                (*new_img)(Point({x, y})) = image_(p);
            }
        }
    }

    image_.swap(*new_img);
}


ImgEff::TriTransform ImgEff::get_back_mapping(Point from, Point dest)
{
    ValT w = cols();
    ValT h = rows();
    PointH top_left     = {0,     0,     1};
    PointH top_right    = {w - 1, 0,     1};
    PointH bottom_left  = {0,     h - 1, 1};
    PointH bottom_right = {w - 1, h - 1, 1};

    PointH from_h = {ValT(from.x), ValT(from.y), 1};
    PointH dest_h = {ValT(dest.x), ValT(dest.y), 1};

    Triangle t11, t12, t13, t14;
    t11 << from_h, top_left,     top_right;
    t12 << from_h, top_right,    bottom_right;
    t13 << from_h, bottom_right, bottom_left;
    t14 << from_h, bottom_left,  top_left;
    std::vector<Triangle> tri_from = {t11, t12, t13, t14};

    Triangle t21(t11), t22(t12), t23(t13), t24(t14);
    std::vector<Triangle> tri_to = {t21, t22, t23, t24};
    for (auto & i : tri_to) {
        i.row(0) << dest_h;
    }
    assert(tri_to[0].row(0) == dest_h);


    TriTransform map;
    map.reserve(4);
    for (SizeT i = 0; uint64_t(i) < tri_to.size(); ++i) {
        // from = to * mtr
        Matrix mtr = tri_to[i].inverse() * tri_from[i];
        map.push_back(std::make_pair(tri_to[i], mtr));
    }
    return map;
}


ImgEff::Matrix ImgEff::find_triangle(TriTransform const & map, Point p)
{
    for (auto const & i : map) {
        // Move to the zero of coordinates
        Matrix translate;
        translate << 1,              0,              0,
                0,                   1,              0,
                -i.first(0, 0),      -i.first(0, 1), 1;
        Triangle triangle = i.first * translate;
        auto b = convert(triangle.row(1));
        auto c = convert(triangle.row(2));

        Vector rh;
        rh << p.x, p.y, 1;
        rh *= translate;
        auto r = convert(rh);

        ValT m = ValT(r.x*b.y - b.x*r.y) / (c.x*b.y - b.x*c.y);
        if (m >= 0 && m <= 1) {
            ValT l = (r.x - m*c.x) / b.x;
            if (l >= 0 && m + l <= 1) {
                return i.second;
            }
        }
    }

    return Matrix::Zero();
}
