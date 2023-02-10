#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
//#include <imgui/imgui_impl_sdl.h>
//#include <imgui/imgui_impl_sdlrenderer.h>
#include <sol/sol.hpp>
#include "../Systems/MovementSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DebugCollisionSystem.h"
#include "../Systems/KeyBoardMovementSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyBoardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include <iostream>
#include <fstream>
#include <set>
#include "../Logger/Logger.h"

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
	//const Uint8* keys = SDL_GetKeyboardState(NULL);

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

		// Handle core SDL events
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				eventManager->emitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
					isRunning = false;
				if (sdlEvent.key.keysym.sym == SDLK_d)
					debugCollision = !debugCollision;
				break;
			case SDL_KEYUP:
				eventManager->emitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
			default:
				break;
		}

		// TODO
		//if (keys[SDL_SCANCODE_ESCAPE])
		//	isRunning = false;
		//else
		//	eventManager->emitEvent<KeyReleasedEvent>(SDL_SCANCODE_ESCAPE);

		//if (keys[SDL_SCANCODE_UP])
		//	eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_UP);
		//if (keys[SDL_SCANCODE_RIGHT])
		//	eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_RIGHT);
		//if (keys[SDL_SCANCODE_DOWN])
		//	eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_DOWN);
		//if (keys[SDL_SCANCODE_LEFT])
		//	eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_LEFT);

		//if (keys[SDL_SCANCODE_SPACE])
		//	eventManager->emitEvent<KeyPressedEvent>(SDL_SCANCODE_SPACE);
	}
}

void Game::LoadLevel(int level) 
{
    // Add the sytems that need to be processed in our game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<DebugCollisionSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyBoardMovementSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitSystem>();
	registry->AddSystem<ProjectileLifecycleSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<RenderHealthSystem>();

	assetManager->AddTexture(renderer, "tilemap-image", "assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "tank-image", "assets/images/tank-panther-left.png");
	assetManager->AddTexture(renderer, "chopper-image", "assets/images/chopper-spritesheet.png");
	assetManager->AddTexture(renderer, "radar-image", "assets/images/radar.png");
	assetManager->AddTexture(renderer, "bullet-image", "assets/images/bullet.png");
	assetManager->AddTexture(renderer, "tree-image", "assets/images/tree.png");
	assetManager->AddFont("charriot-font", "assets/fonts/charriot.ttf", 20);
	assetManager->AddFont("arial-font", "assets/fonts/arial.ttf", 10);
	// Load the tilemap
    int tileSize = 32;
    double tileScale = 1.5;
    int mapNumCols = 1;
    static int mapNumRows = 0;
	std::string fileRow = "";
	size_t pos = 0;

    std::fstream mapFile;
    mapFile.open("assets/tilemaps/jungle.map");
	
	while (getline(mapFile, fileRow))
	{
		while (pos < fileRow.size())
		{
			char ch;
			mapFile.get(ch);
			if (ch == ',')
				mapNumCols++;
			pos++;
		}
		mapNumRows++;
	}
	mapFile.close();

	mapFile.open("assets/tilemaps/jungle.map");
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
			tile.setGroup("tiles");
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();

	mapWidth = mapNumCols * tileSize * tileScale;
	mapHeight = mapNumRows * tileSize * tileScale;

	Entity chopper = registry->CreateEntity();
	chopper.setTag("player");
	chopper.AddComponent<TransformComponent>(glm::vec2(20.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 3);
	chopper.AddComponent<AnimationComponent>(2, 10, true);
	chopper.AddComponent<BoxColliderComponent>(32, 32);
	chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 4000, 30, true);
	chopper.AddComponent<KeyBoardControlledComponent>(glm::vec2(0, -100), glm::vec2(100, 0), glm::vec2(0, 100), glm::vec2(-100, 0));
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(100);
	chopper.AddComponent<TextLabelComponent>(glm::vec2(0), "", "arial-font");

    Entity tank = registry->CreateEntity();
	tank.setGroup("enemies");
    tank.AddComponent<TransformComponent>(glm::vec2(400.0, 50.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	tank.AddComponent<BoxColliderComponent>(32, 32);
	//tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(0, 150.0), 1000, 2000, 30, false);
	tank.AddComponent<HealthComponent>(100);
	tank.AddComponent<TextLabelComponent>(glm::vec2(0), "", "arial-font");

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(700.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2, true);
	radar.AddComponent<AnimationComponent>(8, 5, true);

	Entity tree = registry->CreateEntity();
	tree.setGroup("obstacles");
	tree.AddComponent<TransformComponent>(glm::vec2(500, 50.0), glm::vec2(1.0, 1.0), 0.0);
	tree.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	tree.AddComponent<BoxColliderComponent>(16, 32);
	//tree.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));

	Entity tree2 = registry->CreateEntity();
	tree2.setGroup("obstacles");
	tree2.AddComponent<TransformComponent>(glm::vec2(300, 45.0), glm::vec2(1.0, 1.0), 0.0);
	tree2.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	tree2.AddComponent<BoxColliderComponent>(16, 32);
	//tree2.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));

	Entity label = registry->CreateEntity();
	SDL_Color white = { 255, 255, 255 };
	label.AddComponent<TextLabelComponent>(glm::vec2(windowWidth / 2 - 40, 10), "CHOPPER 1.0", "charriot-font", white, true); 
}


void	Game::Setup()
{
	LoadLevel(1);
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

	// TODO - Take this off this Update Function, subscriptions should only happen once in a setup
	// part of the code, not inside a loop thousand of times.
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventManager);
	registry->GetSystem<MovementSystem>().SubscribeToEvents(eventManager);
	registry->GetSystem<KeyBoardMovementSystem>().SubscribeToEvents(eventManager);
	registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventManager);

	//Ask all the systems to update
	registry->GetSystem<MovementSystem>().Update(deltaT);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventManager);
	registry->GetSystem<ProjectileEmitSystem>().Update(registry);
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectileLifecycleSystem>().Update();

	//Update the registry to process the entities that are waiting to be created/killed
	registry->Update();
}

void	Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// Invoke all the sysems that need to render
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
