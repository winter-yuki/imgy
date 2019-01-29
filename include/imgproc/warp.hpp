#ifndef INCLUDE_IMGPROC_WARP_HPP
#define INCLUDE_IMGPROC_WARP_HPP

#include <vector>

#include <eigen3/Eigen/Core>

#include "include/file/filebase.hpp"
#include "include/imgproc/types.hpp"


namespace ImageProcessing
{

class Warp final {
public:
    explicit Warp(IImgFile & image);

    void warp(Point from, Point dest);


private:
    using Matrix       = Eigen::Matrix<Double, 3, 3>;
    using PointH       = Eigen::Matrix<Double, 1, 3>;
    using Vector       = Eigen::Matrix<Double, 1, 3>;
    using Triangle     = Matrix;
    using TriTransform = std::vector<std::pair<Triangle, Matrix>>;

private:
    TriTransform   get_back_mapping(Point from, Point dest);
    Matrix find_triangle(TriTransform const & map, Point p);

    static Point  convert(PointH const & p);
    static Double norm(Vector v);

private:
    IImgFile & image_;
};


inline Point Warp::convert(PointH const & p) {
    return { SizeT(p[0] / p[2]), SizeT(p[1] / p[2]) };
}

inline Double Warp::norm(Vector v) {
    return std::sqrt(v[0]*v[0] + v[1]*v[1]);
}

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_WARP_HPP
