#ifndef INCLUDE_FILE_FILEBASE_HPP
#define INCLUDE_FILE_FILEBASE_HPP

#include <string>

#include "include/file/types.hpp"


class IImgFile
        : public ImgTypes {
public:
    IImgFile()                             = default;
    virtual ~IImgFile()                    = default;
    IImgFile(IImgFile const &)             = delete;
    IImgFile & operator=(IImgFile const &) = delete;
    IImgFile(IImgFile &&)                  = delete;
    IImgFile & operator=(IImgFile &&)      = delete;

    virtual void print(std::string const & filename) = 0;

    virtual void resize(SizeT new_rows, SizeT new_cols) = 0;

    virtual void       swap (IImgFile & other) = 0;
    virtual IImgFile * clone()                 = 0;

    virtual Pixel       & operator()(SizeT row, SizeT col)       = 0;
    virtual Pixel       & operator()(Point p)                    = 0;
    virtual Pixel const & operator()(SizeT row, SizeT col) const = 0;
    virtual Pixel const & operator()(Point p)              const = 0;

    virtual SizeT rows() const = 0;
    virtual SizeT cols() const = 0;
};


#endif // INCLUDE_FILE_FILEBASE_HPP
