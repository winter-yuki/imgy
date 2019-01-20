#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "include/imgproc/imgproc.hpp"


namespace
{
void test();
} // namespace



int main(int argc, char *argv[])
{
    try {
        //        parse(argc, argv);
        test();
    } catch (std::exception const & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}


namespace
{
void test()
{
    std::string path = "../test/";
    ImgBmp img(path + "1.bmp");

    img.print(path + "10.bmp");
    std::shared_ptr<ImgBmp> p(img.clone());
    img.swap(*p);
    p->print(path + "20.bmp");
    img.print(path + "30.bmp");

    ImgProc img_wrap(img);
    img_wrap.negative();
    img.print(path + "40.bmp");
    img_wrap.error_diffusion();
    img.print(path + "50.bmp");

    img_wrap.warp({img_wrap.cols() / 2, img_wrap.rows() / 2},
    {img_wrap.cols() * 3 / 4, img_wrap.rows() * 3 / 4});
    img.print(path + "60.bmp");


    ImgBmp immgg(100, 500);
    immgg.print(path + "70.bmp");
}
} // namespace
