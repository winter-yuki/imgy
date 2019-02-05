#ifndef INCLUDE_RENDER_TEXTURES_HPP
#define INCLUDE_RENDER_TEXTURES_HPP

#include "include/utility/color.hpp"
#include "include/render/types.hpp"


namespace Render
{

class IMapping {
public:
    IMapping(IMapping const &) = delete;
    IMapping & operator=(IMapping const &) = delete;

    virtual Color operator()(Vector point) = 0;
};




} // namespace Render


#endif // INCLUDE_RENDER_TEXTURES_HPP
