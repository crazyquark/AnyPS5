#ifndef CORE_LIBS_PRX_LIBSCEPAD_INPUTCONFIG_HPP
#define CORE_LIBS_PRX_LIBSCEPAD_INPUTCONFIG_HPP

#include <vector>
#include "prx/libScePad/include/InputMapping.hpp"

namespace Pad {

// The active input mapping: DefaultInputMapping overridden by whatever is in
// controls.cfg next to the executable (created with defaults on first run,
// see InputConfig.cpp). Loaded once, lazily, and cached for the process.
const std::vector<InputBinding>& GetInputMapping();

}

#endif
