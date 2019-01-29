#ifndef INCLUDE_IMGPROC_LUT_HPP
#define INCLUDE_IMGPROC_LUT_HPP

#include <functional>
#include <vector>

#include "include/file/filebase.hpp"
#include "include/imgproc/types.hpp"


namespace ImageProcessing
{

class LUTeffects final {
public:
    explicit LUTeffects(IImgFile & image);

    void negative        ();
    void brightness      (Double coef);
    void contrast        (Double a, Double b);
    void gamma_correction(Double gamma);


private:
    using LUTTable = std::vector<RawCP>;
    using LUTFunc  = std::function<RawCP (RawCP)>;

private:
    void lut_apply(LUTTable const & table);
    static LUTTable lut_get_matr        (LUTFunc const & f);
    static LUTTable lut_negative        ();
    static LUTTable lut_brightness      (Double coef);
    static LUTTable lut_contrast        (Double a, Double b);
    static LUTTable lut_gamma_correction(Double gamma);

private:
    IImgFile & image_;
};

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_LUT_HPP
