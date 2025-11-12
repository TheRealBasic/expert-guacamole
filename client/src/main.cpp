#include "client/client_app.hpp"
#include <cstdint>
#include <iostream>

int main(int argc, char** argv) {
    client::ClientConfig config;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ip" && i + 1 < argc) {
            config.host = argv[++i];
        } else if (arg == "--port" && i + 1 < argc) {
            config.port = static_cast<std::uint16_t>(std::stoi(argv[++i]));
        } else if (arg == "--name" && i + 1 < argc) {
            config.name = argv[++i];
        }
    }

    client::ClientApp app(config);
    if (!app.start()) {
        std::cerr << "Failed to start client\n";
        return 1;
    }
    app.run();
    return 0;
}
