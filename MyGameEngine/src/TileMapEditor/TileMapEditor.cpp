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
	isRunning = true;
}

void TileMapEditor::Run()
{
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

	while (SDL_PollEvent(&sdlEvent))
	{
		if (sdlEvent.type == SDL_QUIT)
			isRunning = false;
		if (keys[SDL_SCANCODE_ESCAPE])
			isRunning = false;
	}
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
}

void TileMapEditor::Update()
{
	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		Logger::Log("X = " + std::to_string(mouseX));
		Logger::Log("Y = " + std::to_string(mouseY));
		SDL_Delay(100);
	}
}

void TileMapEditor::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

void TileMapEditor::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
