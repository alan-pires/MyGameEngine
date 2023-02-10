#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
#include "../Events/CollisionEvent.h"
#include "../EventManager/EventManager.h"

class DamageSystem : public System
{
	public:
		DamageSystem()
		{
			RequireComponent<BoxColliderComponent>();
		}

		void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
		{
			eventManager->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
		}

		void onCollision(CollisionEvent& event)
		{
			Entity a = event.a;
			Entity b = event.b;
			Logger::Log("The Damage system received an event Collision between entities " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));

			if (a.BelongsToGroup("projectiles") && b.HasTag("player"))
			{
				onProjectileHitsPlayer(a, b);
			}

			if (b.BelongsToGroup("projectiles") && a.HasTag("player"))
			{
				onProjectileHitsPlayer(b, a);
			}

			if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies"))
			{
				onProjectileHitsEnemy(a, b);
			}

			if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies"))
			{
				onProjectileHitsEnemy(b, a);
			}
		}

		void onProjectileHitsPlayer(Entity projectile, Entity player)
		{
			const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
			if (!projectileComponent.isFriendly)
			{
				auto &health = player.GetComponent<HealthComponent>();
				health.healthPercentage -= projectileComponent.hitPercentDamage;
				Logger::Log("Health: " + std::to_string(health.healthPercentage));
				if (health.healthPercentage <= 0)
					player.Kill();
				projectile.Kill();
			}
		}

		void onProjectileHitsEnemy(Entity projectile, Entity enemy)
		{
			const auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
			if (projectileComponent.isFriendly)
			{
				auto& health = enemy.GetComponent<HealthComponent>();
				health.healthPercentage -= projectileComponent.hitPercentDamage;
				if (health.healthPercentage <= 0)
					enemy.Kill();
				projectile.Kill();
			}
		}

		void Update()
		{
			
		}
};

#endif