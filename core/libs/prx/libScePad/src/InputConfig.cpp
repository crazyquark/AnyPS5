#include "prx/libScePad/include/InputConfig.hpp"

#include "SDL.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Pad {

namespace {

constexpr const char* ConfigFileName = "controls.cfg";

// Stable name for each user-remappable entry in Pad::InputMapping, by index.
// Bindings not listed here (the scroll-wheel D-pad triggers) aren't
// key/mouse-driven and so aren't remappable via controls.cfg.
struct NamedBinding {
    std::size_t index;
    const char* name;
};

constexpr std::array RemappableBindings{
    NamedBinding{0, "toggle_fullscreen"},
    NamedBinding{1, "cross"},
    NamedBinding{2, "options"},
    NamedBinding{3, "triangle"},
    NamedBinding{4, "circle"},
    NamedBinding{5, "square"},
    NamedBinding{6, "l1"},
    NamedBinding{7, "r1"},
    NamedBinding{8, "l3"},
    NamedBinding{9, "l3_alt"},
    NamedBinding{10, "r3"},
    NamedBinding{11, "r3_alt"},
    NamedBinding{12, "dpad_up"},
    NamedBinding{13, "dpad_right"},
    NamedBinding{14, "dpad_down"},
    NamedBinding{15, "dpad_left"},
    NamedBinding{16, "stick_left_left"},
    NamedBinding{17, "stick_left_right"},
    NamedBinding{18, "stick_left_up"},
    NamedBinding{19, "stick_left_down"},
    NamedBinding{20, "stick_right_left"},
    NamedBinding{21, "stick_right_right"},
    NamedBinding{22, "stick_right_up"},
    NamedBinding{23, "stick_right_down"},
    NamedBinding{24, "touchpad_left"},
    NamedBinding{25, "touchpad_right"},
    NamedBinding{26, "toggle_mouse"},
    NamedBinding{27, "r2"},
    NamedBinding{28, "r1_alt"},
};

std::string trim(const std::string& s) {
    const auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

std::string mouseButtonName(std::uint8_t button) {
    if (button == SDL_BUTTON_LEFT) return "MOUSE_LEFT";
    if (button == SDL_BUTTON_RIGHT) return "MOUSE_RIGHT";
    if (button == SDL_BUTTON_MIDDLE) return "MOUSE_MIDDLE";
    throw std::runtime_error("InputConfig: unsupported default mouse button");
}

std::uint8_t mouseButtonFromName(const std::string& name) {
    if (name == "MOUSE_LEFT") return SDL_BUTTON_LEFT;
    if (name == "MOUSE_RIGHT") return SDL_BUTTON_RIGHT;
    if (name == "MOUSE_MIDDLE") return SDL_BUTTON_MIDDLE;
    throw std::runtime_error("InputConfig: unknown mouse button \"" + name + "\"");
}

void writeDefaultConfig(const std::filesystem::path& path) {
    std::ostringstream out;
    out << "# AnyPS5 control bindings.\n"
        << "# One \"name = value\" per line. Lines starting with # are ignored.\n"
        << "# value is either an SDL key name (Space, A, Left Shift, F11, ...)\n"
        << "# or one of MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE.\n"
        << "# Delete this file to reset to defaults.\n\n";
    for (const auto& entry : RemappableBindings) {
        const auto& binding = InputMapping[entry.index];
        const std::string value = binding.key != SDL_SCANCODE_UNKNOWN
            ? SDL_GetScancodeName(binding.key)
            : mouseButtonName(binding.mouseButton);
        out << entry.name << " = " << value << "\n";
    }
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("InputConfig: failed to create " + path.string());
    const std::string content = out.str();
    file.write(content.data(), static_cast<std::streamsize>(content.size()));
}

std::vector<InputBinding> loadMapping() {
    std::vector<InputBinding> mapping(InputMapping.begin(), InputMapping.end());
    const std::filesystem::path path = std::filesystem::current_path() / ConfigFileName;

    if (!std::filesystem::exists(path)) {
        writeDefaultConfig(path);
        return mapping;
    }

    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("InputConfig: failed to open " + path.string());

    std::string line;
    while (std::getline(file, line)) {
        const std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') continue;
        const auto separator = trimmed.find('=');
        if (separator == std::string::npos) throw std::runtime_error("InputConfig: malformed line \"" + trimmed + "\"");
        const std::string name = trim(trimmed.substr(0, separator));
        const std::string value = trim(trimmed.substr(separator + 1));

        const auto it = std::find_if(RemappableBindings.begin(), RemappableBindings.end(), [&](const NamedBinding& entry) {
            return name == entry.name;
        });
        if (it == RemappableBindings.end()) throw std::runtime_error("InputConfig: unknown binding name \"" + name + "\"");
        auto& binding = mapping[it->index];

        if (value.rfind("MOUSE_", 0) == 0) {
            binding.mouseButton = mouseButtonFromName(value);
            binding.key = SDL_SCANCODE_UNKNOWN;
        } else {
            const SDL_Scancode scancode = SDL_GetScancodeFromName(value.c_str());
            if (scancode == SDL_SCANCODE_UNKNOWN) throw std::runtime_error("InputConfig: unknown key name \"" + value + "\" for \"" + name + "\"");
            binding.key = scancode;
            binding.mouseButton = 0;
        }
    }
    return mapping;
}

}

const std::vector<InputBinding>& GetInputMapping() {
    static const std::vector<InputBinding> mapping = loadMapping();
    return mapping;
}

}
