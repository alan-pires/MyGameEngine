#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <SDL.h>

struct SpriteComponent
{
	std::string assetId;
	int width;
	int height;
	int z;
	SDL_RendererFlip flip;
	SDL_Rect srcRect;
	bool isFixed;

	SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int z =0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0)
	{
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->z = z;
		this->flip = SDL_FLIP_NONE;
		this->srcRect = {srcRectX, srcRectY, width, height};
		this->isFixed = isFixed;
	}
};

#endif