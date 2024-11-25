#ifndef TILE_MAP_EDITOR_H
#define TILE_MAP_EDITOR_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <map>
#include <vector>

class TileMapEditor
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;
	SDL_Texture* tileMapTexture;
	std::map<int, SDL_Rect> rectMap;
	std::map<int, SDL_Rect> tileMapSources;
	SDL_Rect mapArea;
	SDL_Rect tileSelector;
	std::vector<SDL_Rect> rectEmptyTiles;
	SDL_Rect selectedRect;
	int tileWidth = 32;
	int tileHeight = 32;
	int textureWidth = 320;
	int textureHeight = 96;
	bool isDragging = false;

public:
	TileMapEditor();
	~TileMapEditor();
	void Initialize();
	void Setup();
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer);
	void SelectTile();
	void SetTiletoMap();
	void RenderTilesToSelect();
	void Destroy();
	static int windowWidth;
	static int windowHeight;
	int mouseX, mouseY;
	Uint32 mouseState;
};

#endif