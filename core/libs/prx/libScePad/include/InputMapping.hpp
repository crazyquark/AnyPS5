#ifndef CORE_LIBS_PRX_LIBSCEPAD_INPUTMAPPING_HPP
#define CORE_LIBS_PRX_LIBSCEPAD_INPUTMAPPING_HPP

#include <array>
#include <cstdint>
#include "SDL_scancode.h"
#include "SDL_mouse.h"

namespace Pad {
enum class InputControl {
    Button,
    LeftStickLeft,
    LeftStickRight,
    LeftStickUp,
    LeftStickDown,
    RightStickLeft,
    RightStickRight,
    RightStickUp,
    RightStickDown,
    TouchLeft,
    TouchRight,
    ToggleMouse,
    ToggleFullscreen
};

struct InputBinding {
    SDL_Scancode key;
    std::uint8_t mouseButton;
    InputControl control;
    std::uint32_t button = 0;
    int wheelDirection = 0;
    // Stable identifier used by controls.cfg to remap this binding's key/mouseButton.
    // Left null for bindings that aren't user-remappable (e.g. scroll-wheel triggered ones).
    const char* name = nullptr;
};

// Compiled-in defaults. User overrides are applied on top of this at runtime
// by Pad::GetInputMapping() (see InputConfig.hpp) via controls.cfg.
inline constexpr std::array DefaultInputMapping{
    InputBinding{SDL_SCANCODE_F11, 0, InputControl::ToggleFullscreen, 0, 0, "toggle_fullscreen"},
    InputBinding{SDL_SCANCODE_RETURN, 0, InputControl::Button, 0x4000, 0, "cross"},
    InputBinding{SDL_SCANCODE_ESCAPE, 0, InputControl::Button, 0x8, 0, "options"},
    InputBinding{SDL_SCANCODE_I, 0, InputControl::Button, 0x1000, 0, "triangle"},
    InputBinding{SDL_SCANCODE_C, 0, InputControl::Button, 0x2000, 0, "circle"},
    InputBinding{SDL_SCANCODE_SPACE, 0, InputControl::Button, 0x8000, 0, "square"},
    InputBinding{SDL_SCANCODE_Q, 0, InputControl::Button, 0x400, 0, "l1"},
    InputBinding{SDL_SCANCODE_E, 0, InputControl::Button, 0x800, 0, "r1"},
    InputBinding{SDL_SCANCODE_LSHIFT, 0, InputControl::Button, 0x2, 0, "l3"},
    InputBinding{SDL_SCANCODE_RSHIFT, 0, InputControl::Button, 0x2, 0, "l3_alt"},
    InputBinding{SDL_SCANCODE_LCTRL, 0, InputControl::Button, 0x4, 0, "r3"},
    InputBinding{SDL_SCANCODE_RCTRL, 0, InputControl::Button, 0x4, 0, "r3_alt"},
    InputBinding{SDL_SCANCODE_UP, 0, InputControl::Button, 0x10, 0, "dpad_up"},
    InputBinding{SDL_SCANCODE_RIGHT, 0, InputControl::Button, 0x20, 0, "dpad_right"},
    InputBinding{SDL_SCANCODE_DOWN, 0, InputControl::Button, 0x40, 0, "dpad_down"},
    InputBinding{SDL_SCANCODE_LEFT, 0, InputControl::Button, 0x80, 0, "dpad_left"},
    InputBinding{SDL_SCANCODE_A, 0, InputControl::LeftStickLeft, 0, 0, "stick_left_left"},
    InputBinding{SDL_SCANCODE_D, 0, InputControl::LeftStickRight, 0, 0, "stick_left_right"},
    InputBinding{SDL_SCANCODE_W, 0, InputControl::LeftStickUp, 0, 0, "stick_left_up"},
    InputBinding{SDL_SCANCODE_S, 0, InputControl::LeftStickDown, 0, 0, "stick_left_down"},
    InputBinding{SDL_SCANCODE_F, 0, InputControl::RightStickLeft, 0, 0, "stick_right_left"},
    InputBinding{SDL_SCANCODE_H, 0, InputControl::RightStickRight, 0, 0, "stick_right_right"},
    InputBinding{SDL_SCANCODE_T, 0, InputControl::RightStickUp, 0, 0, "stick_right_up"},
    InputBinding{SDL_SCANCODE_G, 0, InputControl::RightStickDown, 0, 0, "stick_right_down"},
    InputBinding{SDL_SCANCODE_BACKSPACE, 0, InputControl::TouchLeft, 0, 0, "touchpad_left"},
    InputBinding{SDL_SCANCODE_TAB, 0, InputControl::TouchRight, 0, 0, "touchpad_right"},
    InputBinding{SDL_SCANCODE_UNKNOWN, SDL_BUTTON_MIDDLE, InputControl::ToggleMouse, 0, 0, "toggle_mouse"},
    InputBinding{SDL_SCANCODE_UNKNOWN, SDL_BUTTON_LEFT, InputControl::Button, 0x200, 0, "r2"},
    InputBinding{SDL_SCANCODE_UNKNOWN, SDL_BUTTON_RIGHT, InputControl::Button, 0x800, 0, "r1_alt"},
    // Scroll-wheel bindings drive the D-pad directly; not user-remappable via key/mouse.
    InputBinding{SDL_SCANCODE_UNKNOWN, 0, InputControl::Button, 0x10, 1},
    InputBinding{SDL_SCANCODE_UNKNOWN, 0, InputControl::Button, 0x40, -1}
};

inline constexpr int MousePollIntervalMs = 33;
inline constexpr int WheelPressDurationMs = 80;
inline constexpr double MouseSensitivity = 1.0;
}

#endif
