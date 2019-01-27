#include "include/imgproc/imgproc.hpp"

#include <cassert>
#include <exception>
#include <memory>


void ImgProc::free_form_deformation(Point p, SizeT right, SizeT up)
{
    if (p.x >= ffd_pivots_.cols() || p.y >= ffd_pivots_.rows()) {
        throw std::runtime_error("Wrong point number");
    }

    std::shared_ptr<IImgFile> new_img(image_.clone());

    SizeT new_height{}, new_width{};
    std::tie(new_height, new_width) = get_new_size(p, right, up);
    new_img->resize_up(new_height, new_width);

    ImgProc new_img_proc(*new_img);
    new_img_proc.fill_color(BLACK_);
    new_img_proc.shift_pivots(p, right, up);
    new_img_proc.ffd_map(*this);

    swap(new_img_proc);
}


ImgProc::Pivots ImgProc::gen_pivots(SizeT img_w, SizeT img_h) const
{
    Pivots p;
    for (SizeT row = 0; row < p.rows(); ++row) {
        for (SizeT col = 0; col < p.cols(); ++col) {
            p(row, col) = { row * img_h / (p.rows() - 1),
                    col * img_w / (p.cols() - 1) };
        }
    }
    return p;
}


std::tuple<ImgProc::SizeT /*height*/, ImgProc::SizeT /*width*/>
ImgProc::get_new_size(Point p, SizeT right, SizeT up) const
{
    Point to_move = ffd_pivots_(p.y, p.x);
    Point moved{to_move.x += right, to_move.y -= up};

    if (moved.x < 0 || moved.x >= image_.cols() ||
            moved.y < 0 || moved.y >= image_.rows()) {
        return { image_.rows(), image_.cols() };
    }

    SizeT dx{};
    if (moved.x < 0) {
        dx = 0 - moved.x;
    } else {
        assert(moved.x >= image_.cols());
        dx = moved.x - image_.cols();
    }
    SizeT dy{};
    if (moved.y < 0) {
        dy = 0 - moved.y;
    } else {
        assert(moved.y >= image_.rows());
        dy = moved.y - image_.rows();
    }
    // Previous picture can be placed on center.
    dx *= 2;
    dy *= 2;

    return { image_.rows() + dy, image_.cols() + dx };
}


void ImgProc::shift_pivots(Point p, SizeT right, SizeT up)
{
    ffd_pivots_(p.y, p.x).x += right;
    ffd_pivots_(p.y, p.x).y -= up;
}


void ImgProc::ffd_map(ImgProc const & from)
{
    const SizeT row_max = image_.rows() / 2;
    const SizeT col_max = image_.cols()  / 2;
    const ValT STEP_V = 1.0 / row_max;
    const ValT STEP_H = 1.0 / col_max;
    ValT u = 0, v = 0;

    for (SizeT row = 0; row < row_max; ++row, v += STEP_V) {
        for (SizeT col = 0; col < col_max; ++col, u += STEP_H) {
            get_param_point(v, u) = from.get_param_point(v, u);
        }
    }
}


ImgProc::Pixel & ImgProc::get_param_point(ValT v, ValT u)
{
    Eigen::Matrix<PointD, Eigen::Dynamic, Eigen::Dynamic>
            rectangle(ffd_pivots_.rows() - 1, ffd_pivots_.cols() - 1);
    for (SizeT row = 0; row < rectangle.rows(); ++row) {
        for (SizeT col = 0; col < rectangle.cols(); ++col) {
            rectangle(row, col) = bilinear_interp({col, row}, u, v);
        }
    }

    auto point = bilinear_interp(rectangle(0, 0), rectangle(0, 1),
                                 rectangle(1, 0), rectangle(1, 1), u, v);
    return image_(SizeT(point[0]), SizeT(point[1]));
}


ImgProc::Pixel & ImgProc::get_param_point(ValT v, ValT u) const
{
    // TODO(ImgProc::get_param_point overloading)
    Eigen::Matrix<PointD, Eigen::Dynamic, Eigen::Dynamic>
            rectangle(ffd_pivots_.rows() - 1, ffd_pivots_.cols() - 1);
    for (SizeT row = 0; row < rectangle.rows(); ++row) {
        for (SizeT col = 0; col < rectangle.cols(); ++col) {
            rectangle(row, col) = bilinear_interp({col, row}, u, v);
        }
    }

    auto point = bilinear_interp(rectangle(0, 0), rectangle(0, 1),
                                 rectangle(1, 0), rectangle(1, 1), u, v);
    return image_(SizeT(point[0]), SizeT(point[1]));
}


ImgProc::PointD
ImgProc::bilinear_interp(Point p,
                         Param u /*p00 - p01*/, Param v /*p00 - p10*/) const
{
    return bilinear_interp(
    {p.x, p.y}, {p.x + 1, p.y}, {p.x, p.y + 1}, {p.x + 1, p.y + 1}, u, v);
}


ImgProc::PointD
ImgProc::bilinear_interp(PointD const & p00, PointD const & p01,
                         PointD const & p10, PointD const & p11,
                         Param u /*p00 - p01*/, Param v /*p00 - p10*/) const
{
    PointD r0 = p00 * (1 - u) + p01 * u;
    PointD r1 = p10 * (1 - u) + p11 * u;
    return r0 * (1 - v) + r1 * v;
}
