#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

#include "Application/Application.hpp"

namespace {
std::string_view parseStringArg(int argc, char** argv, const std::string_view option, std::string_view fallback)
{
    for (int i = 1; i < argc; ++i) {
        const std::string_view current{argv[i]};
        if (current == option && i + 1 < argc) {
            return argv[i + 1];
        }
    }
    return fallback;
}

std::uint16_t parsePortArg(int argc, char** argv, std::uint16_t fallback)
{
    for (int i = 1; i < argc; ++i) {
        const std::string_view current{argv[i]};
        if ((current == "--port" || current == "-p") && i + 1 < argc) {
            return static_cast<std::uint16_t>(std::stoi(argv[i + 1]));
        }
    }
    return fallback;
}
} // namespace

int main(int argc, char** argv)
{
    const auto host = parseStringArg(argc, argv, "--host", "127.0.0.1");
    const auto port = parsePortArg(argc, argv, 9000);

    Application app{port, host};
    return app.run();
}
