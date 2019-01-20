#ifndef INCLUDE_RENDER_TYPES_HPP
#define INCLUDE_RENDER_TYPES_HPP

#include <cinttypes>


namespace Render
{

using SizeT = int64_t;
using ValT  = double;
inline ValT EPSILON() { static const ValT E = 1e-7; return E; }

} // namespace Render


#endif // INCLUDE_RENDER_TYPES_HPP
