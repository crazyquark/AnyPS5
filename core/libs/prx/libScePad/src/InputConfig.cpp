#include "prx/libScePad/include/InputConfig.hpp"

#include "SDL.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace Pad {

namespace {

constexpr const char* ConfigFileName = "controls.cfg";

std::string trim(const std::string& s) {
    const auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

bool isRemappable(const InputBinding& binding) {
    return binding.name != nullptr && (binding.key != SDL_SCANCODE_UNKNOWN || binding.mouseButton != 0);
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
    for (const auto& binding : DefaultInputMapping) {
        if (!isRemappable(binding)) continue;
        const std::string value = binding.key != SDL_SCANCODE_UNKNOWN
            ? SDL_GetScancodeName(binding.key)
            : mouseButtonName(binding.mouseButton);
        out << binding.name << " = " << value << "\n";
    }
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("InputConfig: failed to create " + path.string());
    const std::string content = out.str();
    file.write(content.data(), static_cast<std::streamsize>(content.size()));
}

std::vector<InputBinding> loadMapping() {
    std::vector<InputBinding> mapping(DefaultInputMapping.begin(), DefaultInputMapping.end());
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

        const auto it = std::find_if(mapping.begin(), mapping.end(), [&](const InputBinding& binding) {
            return isRemappable(binding) && name == binding.name;
        });
        if (it == mapping.end()) throw std::runtime_error("InputConfig: unknown binding name \"" + name + "\"");

        if (value.rfind("MOUSE_", 0) == 0) {
            it->mouseButton = mouseButtonFromName(value);
            it->key = SDL_SCANCODE_UNKNOWN;
        } else {
            const SDL_Scancode scancode = SDL_GetScancodeFromName(value.c_str());
            if (scancode == SDL_SCANCODE_UNKNOWN) throw std::runtime_error("InputConfig: unknown key name \"" + value + "\" for \"" + name + "\"");
            it->key = scancode;
            it->mouseButton = 0;
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
