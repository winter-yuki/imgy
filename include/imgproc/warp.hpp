#ifndef INCLUDE_IMGPROC_WARP_HPP
#define INCLUDE_IMGPROC_WARP_HPP

#include "include/file/filebase.hpp"


class Warp final {
public:
    explicit Warp(IImgFile & image);

    void warp(Point from, Point dest);

private:
    using ValT         = double;
    using Matrix       = Eigen::Matrix<ValT, 3, 3>;
    using PointH       = Eigen::Matrix<ValT, 1, 3>;
    using Vector       = Eigen::Matrix<ValT, 1, 3>;
    using Triangle     = Matrix;
    using TriTransform = std::vector<std::pair<Triangle, Matrix>>;

    TriTransform   get_back_mapping(Point from, Point dest);
    Matrix find_triangle(TriTransform const & map, Point p);
    static Point convert(PointH const & p) {
        return { SizeT(p[0] / p[2]), SizeT(p[1] / p[2]) }; }
    static ValT norm(Vector v) { return std::sqrt(v[0]*v[0] + v[1]*v[1]); }

    IImgFile & image_;
};

#endif // INCLUDE_IMGPROC_WARP_HPP
