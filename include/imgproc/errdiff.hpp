#ifndef INCLUDE_IMGPROC_ERRDIFF_HPP
#define INCLUDE_IMGPROC_ERRDIFF_HPP

#include "include/file/filebase.hpp"


class ErrDiff final {
public:
    ErrDiff(IImgFile & image);

    void error_diffusion();

private:
    IImgFile & image_;
};


#endif // INCLUDE_IMGPROC_ERRDIFF_HPP
