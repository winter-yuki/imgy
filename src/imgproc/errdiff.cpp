#include "include/imgproc/errdiff.hpp"

#include <eigen3/Eigen/Core>

#include "include/utility/color.hpp"


namespace ImageProcessing
{

// TODO (ErrDiff refactor)
// TODO (ErrDiff test)

ErrDiff::ErrDiff(IImgFile & image)
    : image_(image)
{}


void ErrDiff::error_diffusion()
{
    // Set matrix of error separation.
    using ErrDifMtr = Eigen::Array<SizeT, Eigen::Dynamic, Eigen::Dynamic>;
    ErrDifMtr matr(3, 5);
    const SizeT col0_matr_offset = 2;
    matr << 0, 0, 0, 7, 5,
            3, 5, 7, 5, 3,
            1, 3, 5, 3, 1;
    const SizeT  denum     = matr.sum();
    const Double THRESHOLD = 0.45 * COLOR_MAX();


    for (SizeT row0 = 0; row0 < image_.rows(); ++row0) {
        for (SizeT col0 = 0; col0 < image_.cols(); ++col0) {

            // Determine color and error.
            ErrorT error{};
            if (Color(image_(row0, col0)).get_intermediate_color() >= THRESHOLD) {
                error = determ_error(WHITE(), image_(row0, col0), denum);
                image_(row0, col0) = WHITE();
            } else {
                error = determ_error(BLACK(), image_(row0, col0), denum);
                image_(row0, col0) = BLACK();
            }

            // Apply error.
            for (SizeT row = 0; row < matr.rows(); ++row) {
                for (SizeT col = col0_matr_offset + 1;
                     col < matr.cols(); ++col) {

                    const SizeT r = row + row0;
                    const SizeT c = col + col0 - col0_matr_offset;
                    if (r < image_.rows() && r >= 0 &&
                            c < image_.cols() && c >= 0) {
                        image_(r, c) = apply_error(image_(r, c),
                                                   error, matr(row, col));
                    }
                }
            }
        }
    }
}


ErrDiff::ErrorT ErrDiff::determ_error(RawPix painted_to,
                                      RawPix real, SizeT denum)
{
    auto db = COLOR_MAX() - std::abs(painted_to.b - real.b);
    auto dg = COLOR_MAX() - std::abs(painted_to.g - real.g);
    auto dr = COLOR_MAX() - std::abs(painted_to.r - real.r);
    db /= denum;
    dg /= denum;
    dr /= denum;

    if (painted_to == WHITE()) {
        db = -db;
        dg = -dg;
        dr = -dr;
    }

    return {db, dg, dr};
}


RawPix ErrDiff::apply_error(RawPix p, ErrorT error, SizeT coef)
{
    error.b *= coef;
    error.g *= coef;
    error.r *= coef;

    auto apply = [this](RawCP & c, ErrPart error) {
        if (error >= 0) {
            c = RawCP(c + error);
        } else {
            c = (c > -error ? c + error : COLOR_MIN());
        }
    };

    apply(p.b, error.b);
    apply(p.g, error.g);
    apply(p.r, error.r);

    return p;
}

} // namespace ImageProcessing
