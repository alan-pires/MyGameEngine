#ifndef DEBUGCOLLISIONSYSTEM_H
#define DEBUGCOLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include <SDL.h>
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class DebugCollisionSystem : public System 
{
public:
	DebugCollisionSystem()
	{
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}
	void Update(SDL_Renderer *renderer, SDL_Rect& camera)
	{
		for (auto entity: GetSystemEntities())
		{
			auto boxCollider = entity.GetComponent<BoxColliderComponent>();
			auto transform = entity.GetComponent<TransformComponent>();

			SDL_Rect rect;
			rect.x = transform.position.x + boxCollider.offset.x - camera.x;
			rect.y = transform.position.y + boxCollider.offset.y - camera.y;
			rect.w = boxCollider.width * transform.scale.x;
			rect.h = boxCollider.height * transform.scale.y;

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
};

#endif