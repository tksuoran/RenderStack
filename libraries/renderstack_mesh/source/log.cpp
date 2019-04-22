#include "renderstack_mesh/log.hpp"
#include "renderstack_toolkit/platform.hpp"

namespace renderstack
{
namespace mesh
{

renderstack::toolkit::log_category log_geometry_mesh(LOG_COLORIZER_DEFAULT, C_RED, C_GRAY, LOG_WARN);
renderstack::toolkit::log_category log_mesh(LOG_COLORIZER_DEFAULT, C_MAGENTA, C_GRAY, LOG_WARN);

} // namespace mesh
} // namespace renderstack
