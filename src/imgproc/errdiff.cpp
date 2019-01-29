#include "include/imgproc/errdiff.hpp"


void ImgProc::error_diffusion()
{
    // Set matrix of error separation.
    using ErrDifMtr = Eigen::Array<SizeT, Eigen::Dynamic, Eigen::Dynamic>;
    ErrDifMtr matr(3, 5);
    const SizeT col0_matr_offset = 2;
    matr << 0, 0, 0, 7, 5,
            3, 5, 7, 5, 3,
            1, 3, 5, 3, 1;
    const SizeT denum = matr.sum();
    const double THRESHOLD = 0.45 * COLOR_MAX_;


    using  ErrPart = int;
    struct ErrorT { ErrPart b, g, r; };

    auto determ_error = [this, denum](Pixel painted_to, Pixel real) -> ErrorT {
        auto db = COLOR_MAX_ - std::abs(painted_to.b - real.b);
        auto dg = COLOR_MAX_ - std::abs(painted_to.g - real.g);
        auto dr = COLOR_MAX_ - std::abs(painted_to.r - real.r);
        db /= denum;
        dg /= denum;
        dr /= denum;

        if (painted_to == WHITE_) {
            db = -db;
            dg = -dg;
            dr = -dr;
        }

        return {db, dg, dr};
    };

    auto apply_error = [this, denum](Pixel p, ErrorT error, SizeT coef) {
        error.b *= coef;
        error.g *= coef;
        error.r *= coef;

        auto apply = [this](ColorPart & c, ErrPart error) {
            if (error >= 0) {
                c = ColorPart(c + error);
            } else {
                c = (c > -error ? c + error : COLOR_MIN_);
            }
        };

        apply(p.b, error.b);
        apply(p.g, error.g);
        apply(p.r, error.r);

        return p;
    };


    for (SizeT row0 = 0; row0 < rows(); ++row0) {
        for (SizeT col0 = 0; col0 < cols(); ++col0) {

            // Determine color and error.
            ErrorT error{};
            if (PixIntermediateColor(image_(row0, col0)) >= THRESHOLD) {
                error = determ_error(WHITE_, image_(row0, col0));
                image_(row0, col0) = WHITE_;
            } else {
                error = determ_error(BLACK_, image_(row0, col0));
                image_(row0, col0) = BLACK_;
            }

            // Apply error.
            for (SizeT row = 0; row < matr.rows(); ++row) {
                for (SizeT col = col0_matr_offset + 1;
                     col < matr.cols(); ++col) {

                    const SizeT r = row + row0;
                    const SizeT c = col + col0 - col0_matr_offset;
                    if (r < rows() && r >= 0 &&
                            c < cols() && c >= 0) {
                        image_(r, c) = apply_error(image_(r, c),
                                                   error, matr(row, col));
                    }
                }
            }
        }
    }
}
