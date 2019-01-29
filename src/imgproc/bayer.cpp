#include "include/imgproc/bayer.hpp"

#include <functional>

#include "include/utility/color.hpp"


namespace ImageProcessing
{
// TODO(test bayer)
Bayer::Bayer(IImgFile & image)
    : image_(image)
{}


void Bayer::bayer_filter(SizeT matr_size)
{
    auto matr = get_bayer_matrix(matr_size);

    for (SizeT row = 0; row < image_.rows(); row += matr_size) {
        for (SizeT col = 0; col < image_.cols(); col += matr_size) {
            auto n_bayer_matrix = what_matr(matr, row, col);
            apply_matr(matr, n_bayer_matrix, row, col);
        }
    }
}


Bayer::BayerMtr Bayer::get_bayer_matrix(SizeT size)
{
    BayerMtr newMatr = BayerMtr(2, 2);
    newMatr << 0, 2,
            3, 1;

    for (SizeT size_curr = newMatr.rows() * 2;
         size_curr <= size; size_curr *= 2) {
        newMatr = get_next_matr(std::move(newMatr), size_curr / 2);
    }

    return newMatr;
}


Bayer::BayerMtr Bayer::get_next_matr(BayerMtr mtr, SizeT size)
{
    auto new_size = size * 2;
    BayerMtr newMtr(new_size, new_size);

    for (SizeT i = 0; i < size; ++i) {
        for (SizeT j = 0; j < size; ++j) {
            auto src = mtr(i, j);
            newMtr(0 + i,     0 + j)    = src * new_size + 0;
            newMtr(0 + i,     size + j) = src * new_size + 2;
            newMtr(size + i, 0 + j)     = src * new_size + 3;
            newMtr(size + i, size + j)  = src * new_size + 1;
        }
    }
    return newMtr;
}


SizeT Bayer::what_matr(BayerMtr const & matr, SizeT row0, SizeT col0)
{
    SizeT  n_all = 0;
    Double sum   = 0;

    SizeT size = matr.rows();
    const SizeT ROW_MAX = image_.rows() - row0;
    const SizeT COL_MAX = image_.cols() - col0;
    for (SizeT row = 0; row < size && row < ROW_MAX; ++row) {
        for (SizeT col = 0; col < size && col < COL_MAX; ++col) {
            sum += Color(image_(row0 + row, col0 + col)).get_intermediate_color();
            ++n_all;
        }
    }

    sum /= n_all * COLOR_MAX();
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
                    (matr(row, col) <= n_matr ? BLACK() : WHITE());
        }
    }
}


} // namespace ImageProcessing






































