#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "include/file/bmp.hpp"
#include "include/imgproc/imgproc.hpp"
#include "include/render/figures.hpp"
#include "include/render/light.hpp"
#include "include/render/noise.hpp"
#include "include/render/render.hpp"
#include "include/render/textures.hpp"
#include "include/render/types.hpp"


namespace
{

void test_proc();
void test_render();

} // namespace



int main(int /*argc*/, char */*argv*/[])
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

        std::string operation;
        switch (i) {
        case 0:
            img.negative();
            operation = "negative";
            std::cout << operation << std::endl;
            break;

        case 1:
            img.brightness(0.2);
            operation = "brightness";
            std::cout << operation << std::endl;
            break;

        case 2:
            img.contrast(20, 100);
            operation = "contrast";
            std::cout << operation << std::endl;
            break;

        case 3:
            img.gamma_correction(0.5);
            operation = "gamma_correction";
            std::cout << operation << std::endl;
            break;

        case 4:
            img.bayer_filter(4);
            operation = "bayer_filter";
            std::cout << operation << std::endl;
            break;

        case 5:
            img.error_diffusion();
            operation = "error_diffusion";
            std::cout << operation << std::endl;
            break;

        case 6:
            img.warp({img.cols() / 2, img.rows() / 2},
            {img.cols() * 3 / 4, img.rows() * 3 / 4});
            operation = "warp";
            std::cout << operation << std::endl;
            break;

        case 7:
            img.free_form_deformation({2, 2}, 100, 50);
            operation = "free_form_deformation";
            std::cout << operation << std::endl;
            break;

        default:
            assert(false);
        }

        tmp->print(rez + std::to_string(i) + "_" + operation + ".bmp");
    }
}


void test_render()
{
    using Vector = Render::Vector;
    std::string path = "../test/tmp/";
    std::string RTIMG = "../../RTIMG/";
    std::string tpath = "../test/src/";
    ImgBmp img(500, 500);

    ImgBmp texture(tpath + "contrast.bmp");

    Render::Render::Figures figs;

    Render::FigColor c1({0, 0, 100});
    Render::FigSphere sphere1{{ 0, 0, 2 }, 0.5, c1};
    figs.push_back(&sphere1);

    Render::HyperTexture ht({0, 0, 0}, {0, 255, 0}, 0.5,  Render::PerlinNoise());
//    Render::HyperTexture ht({0, 0, 0}, {0, 255, 0}, 0.5, [](Vector const & v) {
//        return std::sin(v[0] * 100);
//    });
    Render::FigSphere sphere2{{ 1, 1, 4 }, 2, ht};
    figs.push_back(&sphere2);
    Render::FigPlane p1({0, 1, 0}, {0, -2, 3}, {100, 0, 0}, {0, 0, 0});
    figs.push_back(&p1);
    Render::FigBox b1({-2, -2, 0}, {-1, -1, 1}, {100, 100, 100}, {0, 0, 0});
    figs.push_back(&b1);


    Render::Render::Lights lts;

    Render::LightPoint lp1({1, 2, 1.1}, 0.5);
    lts.push_back(&lp1);
    Render::LightPoint lp2({-0.3, 1.4, 1}, .1);
    lts.push_back(&lp2);
    Render::LightPoint lp3({-0.6, -0.5, 1}, 1);
//    lts.push_back(&lp3);
    Render::LightAmbient la1(.5);
    lts.push_back(&la1);
    Render::LightDirectional ld1({1, -2, 1}, 1);
//    lts.push_back(&ld1);

    Render::LightSpheric s1({0, 2.3, 0}, 0.5, 1);
//    lts.push_back(&s1);

    Render::Render rnd(img, figs, lts);
    rnd.set_pos({9, 0.5, -3});
    rnd.set_up({0, 1, 0});
    rnd.render();
    img.print(path + "render.bmp");

    std::cout << "render tested" << std::endl;
}

} // namespace
