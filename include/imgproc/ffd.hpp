#ifndef INCLUDE_IMGPROC_FFD_HPP
#define INCLUDE_IMGPROC_FFD_HPP

#include <eigen3/Eigen/Core>

#include "include/file/filebase.hpp"
#include "include/imgproc/types.hpp"


namespace ImageProcessing
{

class FFDeffect final {
public:
    static const SizeT N_PIVOTS_V = 3;
    static const SizeT N_PIVOTS_H = 3;

public:
    explicit FFDeffect(IImgFile & image);

    void free_form_deformation(Point p, SizeT right, SizeT up);


private:
    using PointD = Eigen::Matrix<Double, 1, 2>;
    using Pivots = Eigen::Matrix<Point, N_PIVOTS_V, N_PIVOTS_H>;

private:
    static PointD to_PointD(Point p);
    static Point  to_Point (PointD const & p);

    Pivots gen_pivots(SizeT img_w, SizeT img_h) const;

    void swap(FFDeffect & other);

    std::tuple<SizeT /*height*/, SizeT /*width*/>
    get_new_size(Point p, SizeT right, SizeT up) const;
    void shift_pivots(Point p, SizeT right, SizeT up);
    void ffd_map(FFDeffect const & from);

    RawPix & get_param_point(Double u, Double v);
    RawPix   get_param_point(Double u, Double v) const;
    PointD bilinear_interp(Point p,
                           Double u /*p00 - p01*/, Double v /*p00 - p10*/) const;
    PointD bilinear_interp(PointD const & p00, PointD const & p01,
                           PointD const & p10, PointD const & p11,
                           Double u /*p00 - p10*/, Double v /*p00 - p01*/) const;

private:
    IImgFile & image_;
    Pivots     ffd_pivots_;
};


inline FFDeffect::PointD FFDeffect::to_PointD(Point p) {
    return { p.x, p.y };
}

inline Point FFDeffect::to_Point(const PointD & p) {
    return { SizeT(p[0]), SizeT(p[1]) };
}

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_FFD_HPP
