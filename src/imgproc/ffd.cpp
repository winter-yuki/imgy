#include "include/imgproc/ffd.hpp"

#include <cassert>
#include <exception>
#include <memory>


namespace ImageProcessing
{

FFDeffect::FFDeffect(IImgFile & image)
    : image_     (image)
    , ffd_pivots_(gen_pivots(image.cols(), image.rows()))
{}


void FFDeffect::free_form_deformation(Point p, SizeT right, SizeT up)
{
    if (p.x >= ffd_pivots_.cols() || p.y >= ffd_pivots_.rows()) {
        throw std::runtime_error("Wrong point number");
    }

    std::shared_ptr<IImgFile> new_img(image_.clone());
    FFDeffect new_img_proc(*new_img); // Pivots here are the same

    SizeT new_height{}, new_width{};
    std::tie(new_height, new_width) = get_new_size(p, right, up);
    new_img->resize(new_height, new_width);

    new_img_proc.shift_pivots(p, right, up);
    new_img_proc.ffd_map(*this);

    swap(new_img_proc);
}


FFDeffect::Pivots FFDeffect::gen_pivots(SizeT img_w, SizeT img_h) const
{
    assert(img_h >= 0);
    assert(img_w >= 0);

    Pivots p;
    p.fill({0, 0});
    if (img_h == 0 || img_w == 0) {
        return p;
    }

    --img_w;
    --img_h;
    for (SizeT row = 0; row < p.rows(); ++row) {
        for (SizeT col = 0; col < p.cols(); ++col) {
            p(row, col).x = row * img_h / (p.rows() - 1);
            p(row, col).y = col * img_w / (p.cols() - 1);
        }
    }

    return p;
}


void FFDeffect::swap(FFDeffect & other)
{
    image_.swap(other.image_);
    std::swap(ffd_pivots_, other.ffd_pivots_);
}


std::tuple<SizeT /*height*/, SizeT /*width*/>
FFDeffect::get_new_size(Point p, SizeT right, SizeT up) const
{
    Point to_move = ffd_pivots_(p.y, p.x);
    Point moved{to_move.x += right, to_move.y += up};

    if (moved.x >= 0 && moved.x < image_.cols() &&
            moved.y >= 0 && moved.y < image_.rows()) {
        return { image_.rows(), image_.cols() };
    }

    SizeT dx{};
    if (moved.x < 0) {
        dx = 0 - moved.x;
    } else if (moved.x >= image_.cols()) {
        dx = moved.x - image_.cols();
    }
    SizeT dy{};
    if (moved.y < 0) {
        dy = 0 - moved.y;
    } else if (moved.y >= image_.rows()){
        dy = moved.y - image_.rows();
    }

    return { image_.rows() + dy + 1, image_.cols() + dx + 1 };
}


void FFDeffect::shift_pivots(Point p, SizeT right, SizeT up)
{
    ffd_pivots_(p.y, p.x).x += right;
    ffd_pivots_(p.y, p.x).y += up;
}


void FFDeffect::ffd_map(FFDeffect const & from)
{
    const SizeT row_max = image_.rows() * 2;
    const SizeT col_max = image_.cols() * 2;
    const Double STEP_V = 1.0 / row_max;
    const Double STEP_H = 1.0 / col_max;

    Double v = 0;
    for (SizeT row = 0; row < row_max; ++row, v += STEP_V) {
        Double u = 0;
        for (SizeT col = 0; col < col_max; ++col, u += STEP_H) {
            get_param_point(u, v) = from.get_param_point(u, v);
        }
    }
}


RawPix & FFDeffect::get_param_point(Double u, Double v)
{
    Eigen::Matrix<PointD, Eigen::Dynamic, Eigen::Dynamic>
            new_pivots(ffd_pivots_.rows() - 1, ffd_pivots_.cols() - 1);

    for (SizeT row = 0; row < new_pivots.rows(); ++row) {
        for (SizeT col = 0; col < new_pivots.cols(); ++col) {
            new_pivots(row, col) = bilinear_interp({col, row}, u, v);
        }
    }

    auto point_d = bilinear_interp(new_pivots(0, 0), new_pivots(0, 1),
                                   new_pivots(1, 0), new_pivots(1, 1), u, v);
    auto p = to_Point(point_d);
    if (p.x >= image_.cols() || p.y >= image_.rows() || p.x < 0 || p.y < 0) {
        return image_(0, 0);
    }
    return image_(p);
}


RawPix FFDeffect::get_param_point(Double u, Double v) const
{
    // TODO(const-overloading)
    return const_cast<FFDeffect *>(this)->get_param_point(u, v);
}


FFDeffect::PointD
FFDeffect::bilinear_interp(Point p,
                           Double u /*p00 - p01*/, Double v /*p00 - p10*/) const
{
    return bilinear_interp(to_PointD(ffd_pivots_(p.y, p.x)),
                           to_PointD(ffd_pivots_(p.y, p.x + 1)),
                           to_PointD(ffd_pivots_(p.y + 1, p.x)),
                           to_PointD(ffd_pivots_(p.y + 1, p.x + 1)), u, v);
}


FFDeffect::PointD
FFDeffect::bilinear_interp(PointD const & p00, PointD const & p01,
                           PointD const & p10, PointD const & p11,
                           Double u /*p00 - p10*/, Double v /*p00 - p01*/) const
{
    PointD r0 = p00 * (1 - u) + p01 * u;
    PointD r1 = p10 * (1 - u) + p11 * u;
    return r0 * (1 - v) + r1 * v;
}

} // namespace ImageProcessing
