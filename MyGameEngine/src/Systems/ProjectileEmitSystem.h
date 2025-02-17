#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CameraFollowComponent.h"
#include <SDL.h>

class ProjectileEmitSystem : public System
{
public:
	ProjectileEmitSystem()
	{
		RequireComponent<ProjectileEmitterComponent>();
		RequireComponent<TransformComponent>();
	}
	~ProjectileEmitSystem() = default;

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::onKeyPressed);
	}

	void onKeyPressed(KeyPressedEvent &e)
	{
		if (e.code == SDL_SCANCODE_SPACE)
		{
			for (auto entity : GetSystemEntities())
			{
				//if (entity.HasTag("player"))
				//{
				//	const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
				//	const auto transform = entity.GetComponent<TransformComponent>();
				//	const auto rigidbody = entity.GetComponent< RigidBodyComponent>();
				//		
				//	glm::vec2 projectilePosition = transform.position;
				//	if (entity.HasComponent<SpriteComponent>())
				//	{
				//		const auto sprite = entity.GetComponent<SpriteComponent>();
				//		// initial position of the projectile
				//		projectilePosition.x += (transform.scale.x * sprite.width / 2);
				//		projectilePosition.y += (transform.scale.y * sprite.height / 2);
				//	}

				//	glm::vec2 projectileVelocity = projectileEmitter.projectileVelocity;
				//	int directionX = 0;
				//	int directionY = 0;
				//	if (rigidbody.velocity.x > 0) directionX = +1;
				//	if (rigidbody.velocity.x < 0) directionX = -1;
				//	if (rigidbody.velocity.y > 0) directionY = +1;
				//	if (rigidbody.velocity.y < 0) directionY = -1;
				//	projectileVelocity.x = projectileEmitter.projectileVelocity.x * directionX;
				//	projectileVelocity.y = projectileEmitter.projectileVelocity.y * directionY;

				//	Entity projectile = entity.registry->CreateEntity();
				//	projectile.setGroup("projectiles");
				//	projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				//	projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
				//	projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				//	projectile.AddComponent<BoxColliderComponent>(4, 4);
				//	projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

				//}
			}
		}
	}


	void Update(std::unique_ptr<Registry>& registry) {
		//for (auto entity : GetSystemEntities()) {
		//	auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		//	const auto transform = entity.GetComponent<TransformComponent>();

		//	// If emission frequency is zero, bypass re-emission logic
		//	if (projectileEmitter.repeatFrequency == 0) {
		//		continue;
		//	}

		//	// Check if its time to re-emit a new projectile
		//	if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
		//		glm::vec2 projectilePosition = transform.position;
		//		if (entity.HasComponent<SpriteComponent>()) {
		//			const auto sprite = entity.GetComponent<SpriteComponent>();
		//			projectilePosition.x += (transform.scale.x * sprite.width / 2);
		//			projectilePosition.y += (transform.scale.y * sprite.height / 2);
		//		}

		//		// Add a new projectile entity to the registry
		//		Entity projectile = registry->CreateEntity();
		//		projectile.setGroup("projectiles");
		//		projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
		//		projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
		//		projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
		//		projectile.AddComponent<BoxColliderComponent>(4, 4);
		//		projectile.AddComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.projectileDuration);

		//		// Update the projectile emitter component last emission to the current milliseconds
		//		projectileEmitter.lastEmissionTime = SDL_GetTicks();
		//	}
		//}
	}
};

#endif // !PROJECTILEEMITSYSTEM_H

