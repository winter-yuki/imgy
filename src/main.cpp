#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "include/file/bmp.hpp"
#include "include/imgproc/imgproc.hpp"
#include "include/render/figures.hpp"
#include "include/render/render.hpp"


namespace
{

void test_proc();
void test_render();

} // namespace



int main(int argc, char *argv[])
{
    try {
        //        parse(argc, argv);
        test_proc();
//        test_render();
    } catch (std::exception const & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}


namespace
{

void test_proc()
{
    std::string path = "../test/";
    ImgBmp img(path + "tiger.bmp");

//    img.print(path + "10.bmp");
//    std::shared_ptr<ImgBmp> p(img.clone());
//    img.swap(*p);
//    p->print(path + "20.bmp");
//    img.print(path + "30.bmp");

    ImgProc img_wrap(img);
//    img_wrap.negative();
//    img.print(path + "40.bmp");
//    img_wrap.error_diffusion();
//    img.print(path + "50.bmp");

    img_wrap.free_form_deformation({2, 2}, 100, -150);
    img.print(path + "100.bmp");

//    img_wrap.warp({img_wrap.cols() / 2, img_wrap.rows() / 2},
//    {img_wrap.cols() * 3 / 4, img_wrap.rows() * 3 / 4});
//    img.print(path + "60.bmp");

//    ImgBmp immgg(100, 500);
//    immgg.print(path + "70.bmp");
}


void test_render()
{
    std::string path = "../test/";
    ImgBmp img(500, 500);

    Render::Render::Figures figs;
    Render::FigSphere sphere{{ 0, 0, 3 }, 2, {0, 0, 255}};
    figs.push_back(&sphere);
    Render::Render rnd(img, figs);
    rnd.render();
    img.print(path + "200.bmp");
}

} // namespace
