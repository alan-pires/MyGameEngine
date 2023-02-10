#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>
#include "../AssetManager/AssetManager.h"

class RenderSystem : public System
{
	public:
		RenderSystem()
		{
			RequireComponent<TransformComponent>();
			RequireComponent<SpriteComponent>();
		}

		void Update(SDL_Renderer *renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera)
		{
			struct RenderableEntity
			{
				TransformComponent transfComp;
				SpriteComponent spriteComp;
			};
			std::vector<RenderableEntity> renderableEntities;

			for (auto entity: GetSystemEntities())
			{
				RenderableEntity renderableEntitiy;
				renderableEntitiy.spriteComp = entity.GetComponent<SpriteComponent>();
				renderableEntitiy.transfComp = entity.GetComponent<TransformComponent>();
				
				bool isEntityOutsideCameraView = {
					renderableEntitiy.transfComp.position.x + (renderableEntitiy.spriteComp.width * renderableEntitiy.transfComp.scale.x) < camera.x ||
					renderableEntitiy.transfComp.position.x > camera.x + camera.w ||
					renderableEntitiy.transfComp.position.y + (renderableEntitiy.spriteComp.height * renderableEntitiy.transfComp.scale.y) < camera.y ||
					renderableEntitiy.transfComp.position.y > camera.y + camera.h
				};

				// Cull sprites that are outside the camera view
				if (isEntityOutsideCameraView && !renderableEntitiy.spriteComp.isFixed)
				{
					continue;
				}
				renderableEntities.emplace_back(renderableEntitiy);
			}
			sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& r1, const RenderableEntity& r2){
					return r1.spriteComp.z < r2.spriteComp.z;
			});

			// for (auto entity: GetSystemEntities())
			for (auto entity: renderableEntities)
			{
				const auto transform = entity.transfComp;
				const auto sprite = entity.spriteComp;
		
				//Set the source rectangle of our original sprite textures
				SDL_Rect srcRect = sprite.srcRect;
				// Set the destination rectangle with the x, y position to be rendered
				SDL_Rect dstRect = {
					static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
					static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
					static_cast<int>(sprite.width * transform.scale.x),
					static_cast<int>(sprite.height * transform.scale.y)
				};

				SDL_RenderCopyEx(
					renderer,
					assetManager->GetTexture(sprite.assetId),
					&srcRect,
					&dstRect,
					transform.rotation,
					NULL,
					sprite.flip
				);
			}
		}
};

#endif