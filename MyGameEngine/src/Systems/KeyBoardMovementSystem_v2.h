#ifndef KEYBOARDCMOVEMENTSYSTEM_V2_H
#define KEYBOARDCMOVEMENTSYSTEM_V2_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include "../Components/KeyBoardControlledComponent_v2.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include "../Utils/Enum.h"
#include <math.h>

class KeyBoardMovementSystem_v2 : public System
{
private:
	double dt = 0.0;

public:
	KeyBoardMovementSystem_v2()
	{
		RequireComponent<KeyBoardControlledComponent_v2>();
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}
	~KeyBoardMovementSystem_v2() = default;

	void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
	{
		eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyBoardMovementSystem_v2::onKeyPressedEvent);
	}

	void onKeyPressedEvent(KeyPressedEvent& e)
	{
		for (auto& entity : GetSystemEntities())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& player = entity.GetComponent<KeyBoardControlledComponent_v2>();
			auto& sprite = entity.GetComponent<SpriteComponent>();

			transform.rotation = NULL;

			switch (e.eventType)
			{
			case SDL_KEYDOWN:
				Logger::Log("down");

				if (e.code == SDL_SCANCODE_UP)
				{
					transform.position.y -= player.walkSpeed * dt;
					sprite.active = sprite.assetIds[0];
					player.direction = playerDirection::Up;
				}
				if (e.code == SDL_SCANCODE_RIGHT)
				{
					transform.position.x += player.walkSpeed * dt;
					sprite.active = sprite.assetIds[1];
					player.direction = playerDirection::Right;
				}
				if (e.code == SDL_SCANCODE_DOWN)
				{
					transform.position.y += player.walkSpeed * dt;
					sprite.active = sprite.assetIds[2];
					player.direction = playerDirection::Down;
				}
				if (e.code == SDL_SCANCODE_LEFT)
				{
					transform.position.x -= player.walkSpeed * dt;
					sprite.active = sprite.assetIds[3];
					player.direction = playerDirection::Left;
				}
				break;
			case SDL_KEYUP:
				Logger::Log("up");
				if (e.code == SDL_SCANCODE_UP)
				{
					sprite.active = sprite.assetIds[0][0];
				}
				if (e.code == SDL_SCANCODE_RIGHT)
				{
					sprite.active = sprite.assetIds[1][0];
				}
				if (e.code == SDL_SCANCODE_DOWN)
				{
					sprite.active = sprite.assetIds[2][0];
				}
				if (e.code == SDL_SCANCODE_LEFT)
				{
					sprite.active = sprite.assetIds[3][0];
				}
				break;
			default:
				break;
			}




			int paddingLeft = 10;
			int paddingTop = 10;
			int paddingRight = 50;
			int paddingBottom = 50;

			if (transform.position.x < paddingLeft) {
				transform.position.x = paddingLeft;
				transform.velocity.x = 0;
			}
			else if (transform.position.x > Game::mapWidth - paddingRight) {
				transform.position.x = Game::mapWidth - paddingRight;
				transform.velocity.x = 0;
			}

			if (transform.position.y < paddingTop) {
				transform.position.y = paddingTop;
				transform.velocity.y = 0;
			}
			else if (transform.position.y > Game::mapHeight - paddingBottom) {
				transform.position.y = Game::mapHeight - paddingBottom;
				transform.velocity.y = 0;
			}
		}
	}


	void Update(double deltaT)
	{
		dt = deltaT;
	}
};

#endif