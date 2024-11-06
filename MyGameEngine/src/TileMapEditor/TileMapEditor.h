#ifndef TILE_MAP_EDITOR_H
#define TILE_MAP_EDITOR_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class TileMapEditor
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;

public:
	TileMapEditor();
	~TileMapEditor();
	void	Initialize();
	void	Run();
	void	ProcessInput();
	void	Update();
	void	Render();
	void	Destroy();
	static int windowWidth;
	static int windowHeight;
	int mouseX, mouseY;
	Uint32 mouseState;
};

#endif