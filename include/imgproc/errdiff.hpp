#ifndef INCLUDE_IMGPROC_ERRDIFF_HPP
#define INCLUDE_IMGPROC_ERRDIFF_HPP

#include "include/file/filebase.hpp"
#include "include/imgproc/types.hpp"


namespace ImageProcessing
{

class ErrDiff final {
public:
    explicit ErrDiff(IImgFile & image);

    void error_diffusion();


private:
    using  ErrPart = int32_t;
    struct ErrorT { ErrPart b, g, r; };

private:
    ErrorT determ_error(RawPix painted_to, RawPix real, SizeT denum);
    RawPix apply_error (RawPix p, ErrorT error, SizeT coef);

private:
    IImgFile & image_;
};

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_ERRDIFF_HPP
