#ifndef INCLUDE_IMGPROC_BAYER_HPP
#define INCLUDE_IMGPROC_BAYER_HPP

#include "eigen3/Eigen/Core"

#include "include/file/filebase.hpp"
#include "include/imgproc/types.hpp"


namespace ImageProcessing
{

class Bayer final {
public:
    explicit Bayer(IImgFile & image);

    void bayer_filter(SizeT matr_size);


private:
    using BayerMtr = Eigen::Array<SizeT, Eigen::Dynamic, Eigen::Dynamic>;

    BayerMtr get_bayer_matrix(SizeT size);
    BayerMtr get_next_matr(BayerMtr mtr, SizeT size);
    SizeT    what_matr(BayerMtr const & matr, SizeT row0, SizeT col0);
    void     apply_matr(BayerMtr const & matr, SizeT n_matr,
                        SizeT row0, SizeT col0);

private:
    IImgFile & image_;
};

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_BAYER_HPP
