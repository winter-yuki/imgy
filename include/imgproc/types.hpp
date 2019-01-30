#ifndef INCLUDE_IMGPROC_TYPES_HPP
#define INCLUDE_IMGPROC_TYPES_HPP

#include "include/file/filebase.hpp"


namespace ImageProcessing
{

using SizeT    = IImgFile::SizeT;
using Point    = IImgFile::Point;
using Double   = double;
using RawPix   = IImgFile::Pixel;
using RawColor = IImgFile::RawColor;
using RawCP    = IImgFile::ColorPart;

inline constexpr IImgFile::ColorPart COLOR_MAX() { return IImgFile::COLOR_MAX_; }
inline constexpr IImgFile::ColorPart COLOR_MIN() { return IImgFile::COLOR_MIN_; }

inline constexpr IImgFile::RawColor BLACK() { return IImgFile::BLACK_; }
inline constexpr IImgFile::RawColor WHITE() { return IImgFile::WHITE_; }

} // namespace ImageProcessing


#endif // INCLUDE_IMGPROC_TYPES_HPP
