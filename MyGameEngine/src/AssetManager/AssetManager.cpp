#include "./AssetManager.h"

AssetManager::AssetManager()
{
	Logger::Log("Asset Manager constructor called");
}

AssetManager::~AssetManager()
{
	ClearAssets();
	Logger::Log("Asset Manager destructor called");
}

void AssetManager::ClearAssets()
{
	for (auto texture: textures)
		SDL_DestroyTexture(texture.second);
	textures.clear();

	for (auto font : fonts)
		TTF_CloseFont(font.second);
	fonts.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{

	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_FreeSurface(surface);

	textures.emplace(assetId, texture);
	Logger::Log("New texture add to the Asset Manager whith the id = " + assetId);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId)
{
	return textures[assetId];
}

void AssetManager::AddFont(const std::string& assetId, const std::string& filePath, int fontSize)
{
	fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(const std::string& assetId)
{
	return fonts[assetId];
}
