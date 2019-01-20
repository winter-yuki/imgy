#include <cassert>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "imgeff.hpp"
#include "imgfile.hpp"


namespace
{
void test();
void parse        (int argc, char *argv[]);
void parse_render (int argc, char *argv[]);
void parse_process(int argc, char *argv[]);
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

    //    img.print(path + "10.bmp");
    //    std::shared_ptr<ImgBmp> p(img.clone());
    //    img.swap(*p);
    //    p->print(path + "20.bmp");
    //    img.print(path + "30.bmp");

    ImgEff img_wrap(img);
    //    img_wrap.negative();
    //    img.print(path + "40.bmp");
    //    img_wrap.error_diffusion();
    //    img.print(path + "50.bmp");

    //    img_wrap.warp({img_wrap.cols() / 2, img_wrap.rows() / 2},
    //    {img_wrap.cols() * 3 / 4, img_wrap.rows() * 3 / 4});
    //    img.print(path + "60.bmp");


    ImgBmp immgg(100, 500);
    immgg.print(path + "70.bmp");
}


void parse(int argc, char *argv[])
{
    const size_t min_args = 4;
    if (argc < min_args) {
        throw std::runtime_error("no arguments");
    }

    const std::string render  = "rnd";
    const std::string process = "proc";

    std::string mode = argv[1];

    if (mode == render) {
        parse_render(argc, argv);
    } else if (mode == process) {
        parse_process(argc, argv);
    } else {
        throw std::runtime_error("wrong working mode");
    }
}


void parse_render (int argc, char *argv[])
{
    const size_t n_args_need = 4;
    if (argc != 4) {
        throw std::runtime_error("Wrong number of arguments in render mode.\n"
                                 "Need: mode conf_file rez_file");
    }

    std::string conf = argv[2];
    std::string out  = argv[3];
    // TODO(parse_render)
}


void parse_process(int argc, char *argv[])
{
    // TODO(process parse)
}
} // namespace
