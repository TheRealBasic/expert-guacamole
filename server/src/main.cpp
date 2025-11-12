#include "server/server_app.hpp"
#include <cstdint>
#include <iostream>

int main(int argc, char** argv) {
    server::ServerConfig config;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--port" && i + 1 < argc) {
            config.port = static_cast<std::uint16_t>(std::stoi(argv[++i]));
        } else if (arg == "--maxPlayers" && i + 1 < argc) {
            config.maxPlayers = static_cast<std::size_t>(std::stoi(argv[++i]));
        }
    }

    server::ServerApp app(config);
    if (!app.start()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }
    app.run();
    return 0;
}
