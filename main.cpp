#include <iostream>
#include <memory>

#include "imgfile.hpp"


namespace
{
void test();
} // namespace



int main()
{
    test();
    return 0;
}


namespace {
void test()
{
    std::string path = "../test/";
    ImgBmp img(path + "1.bmp");
    img.print(path + "10.bmp");
    std::shared_ptr<ImgBmp> p(img.clone());
    img.swap(*p);
    p->print(path + "20.bmp");
    img.print(path + "30.bmp");
}
} // namespace
