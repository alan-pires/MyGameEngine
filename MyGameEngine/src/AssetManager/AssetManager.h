#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "../Logger/Logger.h"
#include <SDL_ttf.h>

class AssetManager
{
	private:
		std::map<std::string, SDL_Texture*> textures;
		std::map<std::string, TTF_Font*> fonts;
		// audios
	public:
		AssetManager();
		~AssetManager();

		void ClearAssets();
		void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
		SDL_Texture* GetTexture(const std::string& assetId);

		void AddFont(const std::string& assetId, const std::string& filePath, int fontSize);
		TTF_Font* GetFont(const std::string& assetId);
};

#endif