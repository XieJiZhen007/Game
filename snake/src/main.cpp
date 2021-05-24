#include "Game.h"

#include <iostream>

int main() {
    // std::cout << "sdb" << std::endl;
    Game game;
    int success = game.Init();
    if (success) {
        game.Loop();
    }
    game.Shutdown();
    return 0;
}