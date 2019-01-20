#include "include/imgproc/imgproc.hpp"


namespace
{
double PixIntermediateColor(ImgProc::Pixel p)
{
    return double(int(p.b) + p.g + p.r) / 3.0;
}
} // namespace


ImgProc::ImgProc(IImgFile & image)
    : image_(image)
{}


void ImgProc::fill_color(Color c)
{
    for (SizeT row = 0, h = rows(); row < h; ++row) {
        for (SizeT col = 0, w = cols(); col < w; ++col) {
            image_(row, col) = c;
        }
    }
}


void ImgProc::negative()
{
    lut_apply(lut_negative());
}


void ImgProc::brightness(Param coef)
{
    lut_apply(lut_brightness(coef));
}


void ImgProc::contrast(Param a, Param b)
{
    lut_apply(lut_contrast(a, b));
}


void ImgProc::gamma_correction(Param gamma)
{
    lut_apply(lut_gamma_correction(gamma));
}


void ImgProc::lut_apply(LUTTable const & table)
{
    auto apply = [this, &table](SizeT row, SizeT col) {
        Pixel p = image_(row, col);
        image_(row, col) = {table.at(p.b), table.at(p.g), table.at(p.r)};
    };

    for (SizeT row = 0; row < rows(); ++row) {
        for (SizeT col = 0; col < cols(); ++col) {
            apply(row, col);
        }
    }
}


ImgProc::LUTTable ImgProc::lut_get_matr(LUTFunc const & f)
{
    LUTTable table;
    table.reserve(COLOR_MAX_);
    for (SizeT i = 0; i <= COLOR_MAX_; ++i) {
        table.push_back(f(i));
    }
    return table;
}


ImgProc::LUTTable ImgProc::lut_negative()
{
    return lut_get_matr([](ImgProc::ColorPart c) {
        return ImgProc::COLOR_MAX_ - c; });
}


ImgProc::LUTTable ImgProc::lut_brightness(Param coef)
{
    return lut_get_matr([coef](ImgProc::ColorPart c) {
        return ImgProc::clamp_color(int(c + coef * ImgProc::COLOR_MAX_)); });
}


ImgProc::LUTTable ImgProc::lut_contrast(Param a, Param b)
{
    return lut_get_matr([a, b](ImgProc::ColorPart c) {
        return ImgProc::clamp_color(
                    int(ImgProc::COLOR_MAX_ * (c - a) / (b - a))); });
}


ImgProc::LUTTable ImgProc::lut_gamma_correction(Param gamma)
{
    return lut_get_matr([gamma](ImgProc::ColorPart c) {
        return ImgProc::COLOR_MAX_ *
                std::pow(double(c) / ImgProc::COLOR_MAX_, 1.0 / gamma); });
}


void ImgProc::bayer_filter(SizeT matr_size)
{
    auto what_matr = [this](BayerMtr const & matr, SizeT row0, SizeT col0) {
        SizeT  n_all = 0;
        double sum   = 0;

        SizeT size = matr.rows();
        const SizeT ROW_MAX = rows() - row0;
        const SizeT COL_MAX = cols()  - col0;
        for (SizeT row = 0; row < size && row < ROW_MAX; ++row) {
            for (SizeT col = 0; col < size && col < COL_MAX; ++col) {
                sum += PixIntermediateColor(image_(row0 + row, col0 + col));
                ++n_all;
            }
        }

        sum /= n_all * COLOR_MAX_;
        return int(sum * size * size) + 1;
    };

    auto apply_matr = [this](BayerMtr const & matr, SizeT n_matr,
            SizeT row0, SizeT col0) {
        assert(matr.rows() == matr.cols());

        SizeT size = matr.rows();
        const SizeT ROW_MAX = rows() - row0;
        const SizeT COL_MAX = cols()  - col0;
        for (SizeT row = 0; row < size && row < ROW_MAX; ++row) {
            for (SizeT col = 0; col < size && col < COL_MAX; ++col) {
                image_(row0 + row, col0 + col) =
                        (matr(row, col) <= n_matr ? BLACK_ : WHITE_);
            }
        }
    };


    auto matr = get_bayer_matrix(matr_size);

    for (SizeT row0 = 0; row0 < rows(); row0 += matr_size) {
        for (SizeT col0 = 0; col0 < cols(); col0 += matr_size) {
            auto n_bayer_matrix = what_matr(matr, row0, col0);
            apply_matr(matr, n_bayer_matrix, row0, col0);
        }
    }
}


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


ImgProc::ColorPart ImgProc::clamp_color(int color)
{
    if (color <= 0) {
        return 0;
    }

    if (color > COLOR_MAX_) {
        return COLOR_MAX_;
    }

    return color;
}


ImgProc::BayerMtr ImgProc::get_bayer_matrix(SizeT size)
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


ImgProc::Pixel ImgProc::hsv2rgb(PixelHSV p)
{
    if (p.s == 0) {
        return {p.v, p.v, p.v};
    }

    auto newH = p.h / 60.0;
    auto n = int(newH);
    auto frac = newH - n;
    auto c1 = ColorPart(p.v * (1.0 - p.s));
    auto c2 = ColorPart(p.v * (1.0 - p.s * frac));
    auto c3 = ColorPart(p.v * (1.0 - p.s * (1.0 - frac)));

    switch (n) {
    case 0:
        return {p.v, c3, c1};
    case 1:
        return {c2, p.v, c1};
    case 2:
        return {c1, p.v, c3};
    case 3:
        return {c1, c2, p.v};
    case 4:
        return {c3, c1, p.v};
    case 5:
        return {p.v, c1, c2};
    default:
        assert("HSV2RGV 'n' wrong value" && false);
    }
}


ImgProc::PixelHSV ImgProc::rgb2hsv(Pixel p)
{
    auto maxc  = std::max({p.b, p.g, p.r});
    auto minc  = std::min({p.b, p.g, p.r});
    auto delta = maxc - minc;

    ColorPart s{};
    if (maxc > 0) {
        s = delta / maxc;
    }

    auto v = maxc;
    ColorPart h{};
    if (s == 0) {
        h = 0;
    } else {
        auto rc = (maxc - p.r) / delta;
        auto gc = (maxc - p.g) / delta;
        auto bc = (maxc - p.b) / delta;
        if (p.r == maxc) {
            h = bc - gc;
        } else if (p.g == maxc) {
            h = 2 + rc - bc;
        } else {
            h = 4 + gc - rc;
        }
        h *= 60;
    }

    return {h, s, v};
}




























