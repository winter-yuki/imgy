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
    std::string src  = path + "src/";
    std::string name = "contrast.bmp";
    std::string rez  = path + "tmp/";

    for (size_t i = 0; i < 8; ++i) {
        std::shared_ptr<IImgFile> tmp(new ImgBmp(src + name));

        ImageProcessing::ImgProc img(*tmp);

        switch (i) {
        case 0:
            img.negative();
            std::cout << "negative" << std::endl;
            break;

        case 1:
            img.brightness(0.2);
            std::cout << "brightness" << std::endl;
            break;

        case 2:
            img.contrast(20, 100);
            std::cout << "contrast" << std::endl;
            break;

        case 3:
            img.gamma_correction(0.5);
            std::cout << "gamma_correction" << std::endl;
            break;

        case 4:
            img.bayer_filter(4);
            std::cout << "bayer_filter" << std::endl;
            break;

        case 5:
            img.error_diffusion();
            std::cout << "error_diffusion" << std::endl;
            break;

        case 6:
            img.warp({img.cols() / 2, img.rows() / 2},
            {img.cols() * 3 / 4, img.rows() * 3 / 4});
            std::cout << "warp" << std::endl;
            break;

        case 7:
            img.free_form_deformation({0, 0}, 100, 50);
            std::cout << "free_form_deformation" << std::endl;
            break;

        default:
            assert(false);
        }

        tmp->print(rez + name + " " + std::to_string(i) + ".bmp");
    }
}


void test_render()
{
    std::string path = "../test/tmp/";
    ImgBmp img(500, 500);

    Render::Render::Figures figs;

    Render::FigSphere sphere1{{ 0, 0, 2 }, 0.5, {0, 0, 100}};
    figs.push_back(&sphere1);
    Render::FigSphere sphere2{{ 1, 1, 4 }, 0.8, {0, 100, 0}};
    figs.push_back(&sphere2);
    Render::FigPlane p1({0, 1, 0}, {0, -2, 3}, {100, 0, 0}, {0, 0, 0});
    figs.push_back(&p1);
    Render::FigBox b1({0, 0, 0}, {10, 0.2, 10}, {100, 100, 100}, {0, 0, 0});
    figs.push_back(&b1);


    Render::Render::Lights lts;

    Render::LightPoint lp1({1, 2, 1.1}, 3);
    lts.push_back(&lp1);
    Render::LightPoint lp2({-0.3, 1.4, 1}, 2);
//    lts.push_back(&lp2);
    Render::LightPoint lp3({-0.6, -0.5, 1}, 2);
    lts.push_back(&lp3);
    Render::LightAmbient la1(0.2);
    lts.push_back(&la1);
    //    Render::LightDirectional ld1({0, 0, -1}, 20);
    //    lts.push_back(&ld1);

    Render::Render rnd(img, figs, lts);
//    rnd.set_pos({1.5, 0.2, -2});
    rnd.set_to({0, 0, 1});
    rnd.set_pos({-2, -0.2, -3});
    rnd.render();
    img.print(path + "200.bmp");
}

} // namespace
