#include <iostream>
#include "Game/Game.h"
#include <sol/sol.hpp>

using namespace std;

void GameLoop()
{
	Game game;

	game.Initialize();
	game.Run();
	game.Destroy();
}

void TileMapEditor()
{
    std::cout << "*** TileMapEditor ***";
    std::cin.get();
}

int main(int argc, char* argv[])
{
	int option = -1;
    bool running = true;

    while (running)
    {
        std::cout << "Digit 1 for Game and 2 for TileMap Editor or 0 to quit";
        std::cin >> option;

        if (option == 1 || option == 2 || option == 0)
            running = false;
    }

    if (option == 1)
        GameLoop();
    else if (option == 2)
        TileMapEditor();

	return 0;
}
