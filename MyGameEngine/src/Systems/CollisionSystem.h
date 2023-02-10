#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventManager/EventManager.h"
#include "../Events/CollisionEvent.h"
#include <SDL.h>

class CollisionSystem : public System
{
public:
	CollisionSystem(){
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	};
	void Update(std::unique_ptr<EventManager>& eventManager)
	{
		auto entities = GetSystemEntities();

		// Loop the entities the system is interested in
		for (auto i = entities.begin(); i != entities.end(); i++)
		{
			Entity ent = *i;
			auto entTransform = ent.GetComponent<TransformComponent>();
			auto entCollider = ent.GetComponent<BoxColliderComponent>();

			// Loop the entities that still need to be checked
			for (auto j = i; j != entities.end(); j++)
			{
				Entity leftEnt = *j;
				if(ent == leftEnt)
					continue;

				auto leftEntTransform = leftEnt.GetComponent<TransformComponent>();
				auto leftEntCollider = leftEnt.GetComponent<BoxColliderComponent>();

				bool collided = checkAABBCollision(
					entTransform.position.x + entCollider.offset.x,
					entTransform.position.y + entCollider.offset.y,
					entCollider.width,
					entCollider.width,
					leftEntTransform.position.x + leftEntCollider.offset.x,
					leftEntTransform.position.y + leftEntCollider.offset.y,
					leftEntCollider.width,
					leftEntCollider.width
				);
				if (collided /*&& !leftEnt.BelongsToGroup("tiles") && !ent.BelongsToGroup("tiles")*/)
				{
					Logger::Log("Entity " + std::to_string(ent.GetId()) + " is colliding with " + std::to_string(leftEnt.GetId()));
					eventManager->emitEvent<CollisionEvent>(ent, leftEnt);
				}
			}
		}
	}

	bool checkAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
	{
		return (
			(aX < bX + bW) && (aX + aW > bX) &&
			(aY < bY + bH) && (aY + aH > bY)
		);
	}
};

#endif
