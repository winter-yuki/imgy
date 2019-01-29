#include "include/imgproc/bayer.hpp"

#include <functional>


namespace ImageProcessing
{

void Bayer::bayer_filter(SizeT matr_size)
{
    auto matr = get_bayer_matrix(matr_size);

    for (SizeT row0 = 0; row0 < image_.rows(); row0 += matr_size) {
        for (SizeT col0 = 0; col0 < image_.cols(); col0 += matr_size) {
            auto n_bayer_matrix = what_matr(matr, row0, col0);
            apply_matr(matr, n_bayer_matrix, row0, col0);
        }
    }
}


Bayer::BayerMtr Bayer::get_bayer_matrix(SizeT size)
{
    auto next_matr = [](BayerMtr mtr, SizeT size0) {
        auto size = size0 * 2;
        BayerMtr newMtr(size, size);

        for (SizeT i = 0; i < size0; ++i) {
            for (SizeT j = 0; j < size0; ++j) {
                auto src = mtr(i, j);
                newMtr(0 + i,     0 + j)     = src * size + 0;
                newMtr(0 + i,     size0 + j) = src * size + 2;
                newMtr(size0 + i, 0 + j)     = src * size + 3;
                newMtr(size0 + i, size0 + j) = src * size + 1;
            }
        }
        return newMtr;
    };


    BayerMtr newMatr = BayerMtr(2, 2);
    newMatr << 0, 2,
            3, 1;

    for (SizeT size_curr = newMatr.rows() * 2;
         size_curr <= size; size_curr *= 2) {
        newMatr = next_matr(std::move(newMatr), size_curr / 2);
    }

    return newMatr;
}


Bayer::BayerMtr Bayer::get_next_matr(BayerMtr mtr, SizeT size0)
{

}


SizeT Bayer::what_matr(BayerMtr const & matr, SizeT row0, SizeT col0)
{
    auto pix_intermediate_color = [](...) -> Double {
        return Double(SizeT(p.b) + p.g + p.r) / 3.0;
    };

    SizeT  n_all = 0;
    Double sum   = 0;

    SizeT size = matr.rows();
    const SizeT ROW_MAX = image_.rows() - row0;
    const SizeT COL_MAX = image_.cols() - col0;
    for (SizeT row = 0; row < size && row < ROW_MAX; ++row) {
        for (SizeT col = 0; col < size && col < COL_MAX; ++col) {
            sum += PixIntermediateColor(image_(row0 + row, col0 + col));
            ++n_all;
        }
    }

    sum /= n_all * COLOR_MAX_;
    return SizeT(sum * size * size) + 1;
}


void Bayer::apply_matr(BayerMtr const & matr, SizeT n_matr,
                           SizeT row0, SizeT col0)
{
    assert(matr.rows() == matr.cols());

    SizeT size = matr.rows();
    const SizeT ROW_MAX = image_.rows() - row0;
    const SizeT COL_MAX = image_.cols()  - col0;
    for (SizeT row = 0; row < size && row < ROW_MAX; ++row) {
        for (SizeT col = 0; col < size && col < COL_MAX; ++col) {
            image_(row0 + row, col0 + col) =
                    (matr(row, col) <= n_matr ? BLACK_ : WHITE_);
        }
    }
}


} // namespace ImageProcessing






































