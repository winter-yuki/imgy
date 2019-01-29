#ifndef INCLUDE_IMGPROC_FFD_HPP
#define INCLUDE_IMGPROC_FFD_HPP


class FFDeffect final {
public:
private:
    using PointD = Eigen::Matrix<ValT, 1, 2>;
    static PointD to_PointD(Point p) { return { p.x, p.y };   }
    static Point  to_Point (PointD const & p)
    { return { SizeT(p[0]), SizeT(p[1]) }; }

    Pivots gen_pivots(SizeT img_w, SizeT img_h)  const;
    std::tuple<SizeT /*height*/, SizeT /*width*/>
    get_new_size(Point p, SizeT right, SizeT up) const;
    void shift_pivots(Point p, SizeT right, SizeT up);
    void ffd_map(ImgProc const & from);
    Pixel & get_param_point(ValT u, ValT v);
    Pixel   get_param_point(ValT u, ValT v) const;
    PointD  bilinear_interp(ImgTypes::Point p,
                            Param u /*p00 - p01*/, Param v /*p00 - p10*/) const;
    PointD bilinear_interp(PointD const & p00, PointD const & p01,
                           PointD const & p10, PointD const & p11,
                           Param u /*p00 - p10*/, Param v /*p00 - p01*/) const;

private:
    IImgFile & image_;
    Pivots     ffd_pivots_;
};

#endif // INCLUDE_IMGPROC_FFD_HPP
