#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
//#include <imgui/imgui_impl_sdl.h>
//#include <imgui/imgui_impl_sdlrenderer.h>
#include <imgui/imgui_sdl.h>
#include <sol/sol.hpp>
#include "../Systems/MovementSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DebugCollisionSystem.h"
#include "../Systems/KeyBoardMovementSystem_v1.h"
#include "../Systems/KeyBoardMovementSystem_v2.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthSystem.h"
#include "../Systems/ScriptSystem.h"
#include <iostream>
#include <set>
#include "../Logger/Logger.h"
#include "LevelLoader.h"

int Game::windowHeight;
int Game::windowWidth;
int Game::mapHeight;
int Game::mapWidth;


Game::Game()
{
	isRunning = false;
	registry = std::make_unique<Registry>();
	assetManager = std::make_unique<AssetManager>();
	eventManager = std::make_unique<EventManager>();
	Logger::Log("Game Constructor Called");
}

Game:: ~Game()
{
	Logger::Log("Game Destructor Called");
}

void	Game::Initialize()
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
	windowHeight = 600;

	window = SDL_CreateWindow(
		"Engine",
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

	// init the ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	//ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	//ImGui_ImplSDLRenderer_Init(renderer);

	// Initializes the camera with the entire screen area
	camera.h = windowHeight;
	camera.w = windowWidth;
	camera.x = 0;
	camera.y = 0;

	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}

void	Game::Run()
{
	Setup();
	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void	Game::ProcessInput()
{
	SDL_Event sdlEvent;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	while(SDL_PollEvent(&sdlEvent))
	{
		// ImGui SDL input
		//ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		//ImGuiIO& io = ImGui::GetIO();
		//int mouseX, mouseY;
		//const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		//io.MousePos = ImVec2(mouseX, mouseY);
		//io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		//io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

		if (keys[SDL_SCANCODE_ESCAPE])
			isRunning = false;
		if (keys[SDL_SCANCODE_UP])
			eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_UP, keys);
		if (keys[SDL_SCANCODE_RIGHT])
			eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_RIGHT, keys);
		if (keys[SDL_SCANCODE_DOWN])
			eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_DOWN, keys);
		if (keys[SDL_SCANCODE_LEFT])
			eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_LEFT, keys);
		//if (keys[SDL_SCANCODE_D])
		//	debugCollision = !debugCollision;
	}
}

// Add the sytems that need to be processed in our game
void	Game::AddSystems()
{
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<DebugCollisionSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyBoardMovementSystem_v1>();
	//registry->AddSystem<KeyBoardMovementSystem_v2>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitSystem>();
	registry->AddSystem<ProjectileLifecycleSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<RenderHealthSystem>();
	registry->AddSystem<ScriptSystem>();
}


void	Game::Setup()
{
	AddSystems();

	// Create bindings between C++ and Lua
	registry->GetSystem<ScriptSystem>().CreateLuaBindings(lua);

	LevelLoader loader;
	lua.open_libraries(sol::lib::base, sol::lib::math);
	loader.LoadLevel(lua, registry, assetManager, renderer, 1);
}

void	Game::Update()
{
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
		SDL_Delay(timeToWait);
	double deltaT = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
	// stores the "previous" frame time (ticks after the SDL_Delay)
	millisecsPreviousFrame = SDL_GetTicks();

	eventManager->Reset();

	// URGENTE
	// TODO - Take this off this Update Function, subscriptions should only happen once in a setup
	// part of the code, not inside a loop thousand of times.
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventManager);
	registry->GetSystem<MovementSystem>().SubscribeToEvents(eventManager);
	registry->GetSystem<KeyBoardMovementSystem_v1>().SubscribeToEvents(eventManager);
	//registry->GetSystem<KeyBoardMovementSystem_v2>().SubscribeToEvents(eventManager);
	registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventManager);

	//Ask all the systems to update
	registry->GetSystem<MovementSystem>().Update(deltaT);
	registry->GetSystem<KeyBoardMovementSystem_v1>().Update(deltaT);
	//registry->GetSystem<KeyBoardMovementSystem_v2>().Update(deltaT);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventManager);
	registry->GetSystem<ProjectileEmitSystem>().Update(registry);
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectileLifecycleSystem>().Update();
	registry->GetSystem<ScriptSystem>().Update(deltaT, SDL_GetTicks());

	//Update the registry to process the entities that are waiting to be created/killed
	registry->Update();
}

void	Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// Invoke all the systems that need to render
	registry->GetSystem<RenderSystem>().Update(renderer, assetManager, camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetManager, camera);
	registry->GetSystem<RenderHealthSystem>().Update(renderer, assetManager, camera);

	if (debugCollision)
	{
		registry->GetSystem<DebugCollisionSystem>().Update(renderer, camera);
		
		// Start the Dear ImGui frame
		//ImGui_ImplSDLRenderer_NewFrame();
		//ImGui_ImplSDL2_NewFrame();
		//ImGui::NewFrame();

		//Show demo window
		ImGui::ShowDemoWindow();

		//Render ImGui
		ImGui::Render();
		//ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}

	SDL_RenderPresent(renderer);
}

void	Game::Destroy()
{
	// Cleanup
	//ImGui_ImplSDLRenderer_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
