#ifndef INCLUDE_IMGPROC_LUT_HPP
#define INCLUDE_IMGPROC_LUT_HPP

#include <functional>
#include <vector>

#include "include/file/filebase.hpp"
#include "include/file/types.hpp"


class LUTeffects final
        : public ImgTypes {
public:
    using Param = double;

public:
    explicit LUTeffects(IImgFile & image_);

    void negative        ();
    void brightness      (Param coef);
    void contrast        (Param a, Param b);
    void gamma_correction(Param gamma);


private:
    using LUTTable = std::vector<ColorPart>;
    using LUTFunc  = std::function<ColorPart (ColorPart)>;

private:
    void lut_apply(LUTTable const & table);
    static LUTTable lut_get_matr        (LUTFunc const & f);
    static LUTTable lut_negative        ();
    static LUTTable lut_brightness      (Param coef);
    static LUTTable lut_contrast        (Param a, Param b);
    static LUTTable lut_gamma_correction(Param gamma);

private:
    IImgFile & image_;
};


#endif // INCLUDE_IMGPROC_LUT_HPP
