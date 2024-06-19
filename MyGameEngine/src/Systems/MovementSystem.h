#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventManager/EventManager.h"
#include "../Events/CollisionEvent.h"

class MovementSystem : public System
{
	public:
		MovementSystem()
		{
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
		}
		~MovementSystem() = default;

		void SubscribeToEvents(const std::unique_ptr<EventManager>& eventManager)
		{
			eventManager->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
		}

		void onCollision(CollisionEvent& event)
		{
			Entity a = event.a;
			Entity b = event.b;

			if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles"))
			{
				onEnemyHitsObstacle(a, b);
			}

			if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies"))
			{
				onEnemyHitsObstacle(b, a);
			}
		}

		void onEnemyHitsObstacle(Entity enemy, Entity obstacle)
		{
			if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>())
			{
				auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
				auto& sprite = enemy.GetComponent<SpriteComponent>();

				if (rigidbody.velocity.x)
				{
					sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
					rigidbody.velocity.x *= -1;
				}
				if (rigidbody.velocity.y)
				{
					sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
					rigidbody.velocity.y *= -1;
				}
			}
		}

		void Update(double deltaT)
		{
			for (auto entity : GetSystemEntities())
			{
				if (!entity.HasTag("player"))  // soluçao provisoria para nao afetar movimento do player. Arrumar
				{
					auto& transform = entity.GetComponent<TransformComponent>();
					const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
					transform.position.x += rigidbody.velocity.x * deltaT;
					transform.position.y += rigidbody.velocity.y * deltaT;

					int margin = 100;
					bool isEntityOutsideMap = {
						transform.position.x < -margin ||
						transform.position.x > Game::mapWidth + margin ||
						transform.position.y < -margin ||
						transform.position.y > Game::mapHeight + margin
					};

					if (isEntityOutsideMap)
						entity.Kill();
				}
			}
		}
};

#endif