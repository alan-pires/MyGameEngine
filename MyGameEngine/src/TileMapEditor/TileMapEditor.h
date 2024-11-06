#ifndef TILE_MAP_EDITOR_H
#define TILE_MAP_EDITOR_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>


class TileMapEditor
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;

public:
	TileMapEditor();
	~TileMapEditor();
	void Initialize();
	void Setup();
	void Run();
	void ProcessInput();
	void PrintXY(); // temp
	void Update();
	void Render();
	SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer);
	void RenderTileMap();
	void Destroy();
	static int windowWidth;
	static int windowHeight;
	int mouseX, mouseY;
	Uint32 mouseState;
};

#endif