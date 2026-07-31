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
    GameRun();

	return 0;
}
