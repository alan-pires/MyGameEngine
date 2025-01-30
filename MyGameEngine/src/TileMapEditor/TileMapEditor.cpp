#include "TileMapEditor.h"
#include <iostream>
#include "../Logger/Logger.h"

int TileMapEditor::windowHeight;
int TileMapEditor::windowWidth;

TileMapEditor::TileMapEditor()
{
	isRunning = false;
	Logger::Log("TileMapEditor Constructor Called");
}

TileMapEditor::~TileMapEditor()
{
	Logger::Log("TileMapEditor Destructor Called");
}

void TileMapEditor::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error initializing SDL");
		return;
	}

	if (TTF_Init() != 0)
	{
		Logger::Err("Error initializing TTF");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = 800;
	windowHeight = 800;

	window = SDL_CreateWindow(
		"Tile Map Editor",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		Logger::Err("Error creating SDL Window");
		return;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		Logger::Err("Error creating SDL Renderer");
		return;
	}
	// Initialize SDL2_image for JPG support
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		Logger::Err("Error in IMG_Init");
		return;
	}
	isRunning = true;
}

void TileMapEditor::Setup()
{
	mapArea = { 0, 0, 800, 600 };
	tileMapTexture = LoadTexture("./assets/tilemaps/grass.png", renderer);
	if (!tileMapTexture)
	{
		Logger::Err("Error loading texture");
		return;
	}

	tileSelector = { 10, 645, 320, 96 };

	for (int i = 0; i < 800; i += 32)
	{
		for (int j = 0; j < 600; j += 32)
		{
			SDL_Rect rect = { i, j, 32, 32 };
			rectEmptyTiles.push_back(rect);
		}
	}
}

void TileMapEditor::Run()
{
	Setup();
	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void TileMapEditor::ProcessInput()
{
	SDL_Event sdlEvent;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	while (SDL_PollEvent(&sdlEvent))
	{
		if (sdlEvent.type == SDL_QUIT)
			isRunning = false;
		if (keys[SDL_SCANCODE_ESCAPE])
			isRunning = false;
		if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
			{
				isDragging = true;
				SelectTile();
			}

		if (sdlEvent.type == SDL_MOUSEBUTTONUP)
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
				isDragging = false;
	}
	if (isDragging)
		SetTiletoMap();
}

void TileMapEditor::Update()
{

}

void TileMapEditor::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	RenderTilesToSelect();

	for (int i = 0; i < rectEmptyTiles.size(); i++)
	{
		SDL_Rect& mapTile = rectEmptyTiles[i];

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &mapTile);


		if (tileMapSources.find(i) != tileMapSources.end())
		{
			SDL_Rect& srcRect = tileMapSources[i];

            if (SDL_RenderCopy(renderer, tileMapTexture, &srcRect, &mapTile) != 0)
                Logger::Err("SDL_RenderCopy error: " + std::string(SDL_GetError()));
		}
	}
	SDL_RenderPresent(renderer);
}

SDL_Texture* TileMapEditor::LoadTexture(const char* filePath, SDL_Renderer* renderer)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath);
	if (!surface)
	{
		Logger::Err("Error loading surface: ");
		return nullptr;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		Logger::Err("Error creating Texture from Surface");
		return nullptr;
	}
	//SDL_FreeSurface(surface);
	return texture;
}

void TileMapEditor::RenderTilesToSelect()
{
	int id = 0;

	SDL_RenderCopy(renderer, tileMapTexture, NULL, &tileSelector);

	for (int j = 645; j < 741; j += 32)
	{
		for (int i = 10; i < 330; i += 32)
		{
			rectMap[id] = { i, j, 32, 32 };
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &rectMap[id]);
			id++;
		}
	}
}

void TileMapEditor::SelectTile()
{
	SDL_Point mousePoint = { mouseX, mouseY };

	for (const auto& [id, rect] : rectMap)
	{
		if (SDL_PointInRect(&mousePoint, &rect))
		{
			int tilesPerRow = textureWidth / tileWidth;
			int tileX = (id % tilesPerRow) * tileWidth;
			int tileY = (id / tilesPerRow) * tileHeight; 

			selectedRect = { tileX, tileY, tileWidth, tileHeight };	
			break;
		}
	}
}

void TileMapEditor::SetTiletoMap()
{
	SDL_Point mousePoint = { mouseX, mouseY };
	for (int i = 0; i < rectEmptyTiles.size(); i++)
	{
		SDL_Rect& mapTile = rectEmptyTiles[i];
		if (SDL_PointInRect(&mousePoint, &mapTile))
		{
			if (selectedRect.w > 0 && selectedRect.h > 0) 
				tileMapSources[i] = selectedRect;
			break;
		}
	}
}

void TileMapEditor::Destroy()
{
	SDL_DestroyTexture(tileMapTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
