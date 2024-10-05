#ifndef LEVELLOADER_h
#define LEVELLOADER_H
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include <memory>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& assetManager, SDL_Renderer* renderer, int levelNumber);
private:

};



#endif // !LEVELLOADER_h
