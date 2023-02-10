#ifndef RENDERHEALTHSYSTEM_H
#define RENDERHEALTHSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL2/SDL.h>
#include "../AssetManager/AssetManager.h"

class RenderHealthSystem : public System
{
public:
	RenderHealthSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<HealthComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
	{
		std::string text;
		SDL_Color color;		

		for (auto entity : GetSystemEntities())
		{
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto health = entity.GetComponent<HealthComponent>();

			text = std::to_string(health.healthPercentage) + "%";
			if (health.healthPercentage > 70)
				color = { 0, 255, 0 };
			else if (health.healthPercentage > 40)
				color = { 0, 255, 255 };
			else
				color = { 255, 0, 0 };

			SDL_Surface* surface = TTF_RenderText_Blended(
				assetManager->GetFont("arial-font"),
				text.c_str(),
				color
			);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			int labelWidth = 0;
			int labelHeight = 0;
			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);
			SDL_Rect dstRect = {
				static_cast<int>((transform.position.x + 5) - camera.x),
				static_cast<int>((transform.position.y - 10) - camera.y),
				labelWidth,
				labelHeight
			};
			SDL_RenderCopy(renderer, texture, NULL, &dstRect);
			SDL_DestroyTexture(texture);
		}
	}
};

#endif