#include <iostream>
#include "Game/Game.h"
#include <sol/sol.hpp>
#include "TileMapEditor/TileMapEditor.h"

using namespace std;

void GameRun()
{
	Game game;

	game.Initialize();
	game.Run();
	game.Destroy();
}

void TileMapEditorRun()
{
    TileMapEditor tmap;

    tmap.Initialize();
    tmap.Run();
    tmap.Destroy();
}

int main(int argc, char* argv[])
{
	int option = -1;
    bool running = true;

    while (running)
    {
        std::cout << "Enter 1 for Game and 2 for TileMap Editor or 0 to quit";
        std::cin >> option;

        if (option == 1 || option == 2 || option == 0)
            running = false;
    }

    if (option == 1)
        GameRun();
    else if (option == 2)
        TileMapEditorRun();

	return 0;
}
