#ifndef CORE_LIBS_PRX_LIBSCEPAD_INPUTCONFIG_HPP
#define CORE_LIBS_PRX_LIBSCEPAD_INPUTCONFIG_HPP

#include <vector>
#include "prx/libScePad/include/InputMapping.hpp"

namespace Pad {

// Pad::InputMapping (compile-time defaults) overridden by whatever is in
// controls.cfg next to the executable (created with defaults on first run,
// see InputConfig.cpp). Same size and order as InputMapping always; only
// key/mouseButton can be overridden. Loaded once, lazily, and cached.
const std::vector<InputBinding>& GetInputMapping();

}

#endif
