#include "shadow/game.hpp"
#include <exception>
#include <iostream>

int main() {
    try {
        shadow::Game game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred.\n";
        return 1;
    }
    return 0;
}
