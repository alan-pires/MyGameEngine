#ifndef GAME_H
#define GAME_H

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventManager/EventManager.h"
#include <sol/sol.hpp>

#define FOV_ANGLE (60 * (PI / 180))
#define PI 3.14159265
#define TWO_PI 6.28318530

// #define NUM_RAYS WINDOW_WIDTH

const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000/FPS;

class Game
{
	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Rect camera;
		bool isRunning;

		sol::state lua;
		std::unique_ptr<Registry> registry;
		std::unique_ptr<AssetManager> assetManager;
		std::unique_ptr<EventManager> eventManager;

		bool debugCollision = false;
		int millisecsPreviousFrame = 0;

	public:
		Game();
		~Game();
		void	Initialize();
		void	Run();
		void	ProcessInput();
		void	Update();
		void	Render();
		void	Destroy();
		void	Setup();
		void	AddSystems();
		void	ProcessKeyUpDown();

		static int windowWidth;
		static int windowHeight;
		static int mapWidth;
		static int mapHeight;
};

#endif