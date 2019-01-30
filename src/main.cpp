#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "include/file/bmp.hpp"
#include "include/imgproc/imgproc.hpp"
#include "include/render/figures.hpp"
#include "include/render/light.hpp"
#include "include/render/render.hpp"
#include "include/render/types.hpp"


namespace
{

void test_proc();
void test_render();

} // namespace



int main(int argc, char *argv[])
{
    try {
        //        parse(argc, argv);
        //        test_proc();
        test_render();
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

    // TODO (testing)

    //    img.print(path + "10.bmp");
    //    std::shared_ptr<ImgBmp> p(img.clone());
    //    img.swap(*p);
    //    p->print(path + "20.bmp");
    //    img.print(path + "30.bmp");

//    ImgProc img_wrap(img);
    //    img_wrap.negative();
    //    img.print(path + "40.bmp");
    //    img_wrap.error_diffusion();
    //    img.print(path + "50.bmp");

//    img_wrap.free_form_deformation({2, 2}, 100, -150);
//    img.print(path + "100.bmp");

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

    Render::FigSphere sphere1{{ 0, 0, 2 }, 0.5, {0, 0, 100}};
    figs.push_back(&sphere1);
    Render::FigSphere sphere2{{ 1, 1, 4 }, 0.8, {0, 100, 0}};
    figs.push_back(&sphere2);
    Render::FigPlane p1({0, 1, 0}, {0, -2, 3}, {100, 0, 0});
    figs.push_back(&p1);
    Render::FigBox b1({-2, -2, 2}, {2, 2.3, 3}, {100, 100, 100});
    figs.push_back(&b1);


    Render::Render::Lights lts;

    Render::LightPoint lp1({1, 2, 1.1}, 5);
    lts.push_back(&lp1);
    Render::LightPoint lp2({-1, 2, 2}, 2);
    lts.push_back(&lp2);
    Render::LightAmbient la1(1.05);
//    lts.push_back(&la1);
//    Render::LightDirectional ld1({0, 0, -1}, 20);
//    lts.push_back(&ld1);

    Render::Render rnd(img, figs, lts);
    rnd.render();
    img.print(path + "200.bmp");
}

} // namespace
