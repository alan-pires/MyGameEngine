#include <iostream>
#include "Game/Game.h"

using namespace std;

int main(int argc, char* argv[])
{
	Game game;
	   
	game.Initialize();
	game.Run();
	game.Destroy();

	return 0;
}
