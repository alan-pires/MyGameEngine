#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include "../Components/KeyBoardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include <math.h>

class KeyBoardMovementSystem : public System
{
	public:
		KeyBoardMovementSystem()
		{
			RequireComponent<KeyBoardControlledComponent>();
			RequireComponent<SpriteComponent>();
			RequireComponent<TransformComponent>();
		}
		~KeyBoardMovementSystem() = default;

		void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
		{
			eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyBoardMovementSystem::onKeyPressedEvent);
			eventManager->SubscribeToEvent<KeyReleasedEvent>(this, &KeyBoardMovementSystem::onKeyReleased);
		}

		void onKeyPressedEvent(KeyPressedEvent& e)
		{
			for (auto& entity: GetSystemEntities())
			{
				auto& keyboardControled = entity.GetComponent<KeyBoardControlledComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();

				switch (e.symbol)
				{
					case SDLK_UP:
						keyboardControled.walkDirection = +1;
						//sprite.srcRect.y = sprite.height * 0;
						break;
					case SDLK_RIGHT:
						keyboardControled.turnDirection = +1;
						//sprite.srcRect.y = sprite.height * 1;
						break;
					case SDLK_DOWN:
						keyboardControled.walkDirection = -1;
						//sprite.srcRect.y = sprite.height * 2;
						break;
					case SDLK_LEFT:
						keyboardControled.turnDirection = -1;
						//sprite.srcRect.y = sprite.height * 3;
						break;
				}

				if (entity.HasTag("player"))
				{
					keyboardControled.rotationAngle += keyboardControled.turnDirection * keyboardControled.turnSpeed; // * deltaT

					float moveStep = keyboardControled.walkDirection * keyboardControled.walkSpeed;

					//float newPlayerX = cos(keyboardControled.rotationAngle) * moveStep;
					//float newPlayerY = sin(keyboardControled.rotationAngle) * moveStep;

					//transform.position.x = newPlayerX;
					//transform.position.y = newPlayerY;

					transform.position.x += cos(keyboardControled.rotationAngle) * moveStep;
					transform.position.y += sin(keyboardControled.rotationAngle) * moveStep;

					int paddingLeft = 10;
					int paddingTop = 10;
					int paddingRight = 50;
					int paddingBottom = 50;

					transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
					transform.position.x = transform.position.x > Game::mapWidth - paddingRight ? Game::mapWidth - paddingRight : transform.position.x;
					transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;
					transform.position.y = transform.position.y > Game::mapHeight - paddingBottom ? Game::mapHeight - paddingBottom : transform.position.y;
				}
			}
		}

		void onKeyReleased(KeyReleasedEvent &e)
		{
			for (auto& entity: GetSystemEntities())
			{
				auto& keyboardControl = entity.GetComponent<KeyBoardControlledComponent>();

				switch (e.symbol)
				{
				case SDLK_UP:
					keyboardControl.walkDirection = 0;
					break;
				case SDLK_RIGHT:
					keyboardControl.turnDirection = 0;
					break;
				case SDLK_DOWN:
					keyboardControl.walkDirection = 0;
					break;
				case SDLK_LEFT:
					keyboardControl.turnDirection = 0;
					break;
				}
			}
		}

		void Update(double deltaT)
		{
			/*for (auto entity : GetSystemEntities())
			{
				if (entity.HasTag("player"))  
				{
					auto& keyboardControled = entity.GetComponent<KeyBoardControlledComponent>();
					auto& transform = entity.GetComponent<TransformComponent>();
					
					keyboardControled.rotationAngle += keyboardControled.turnDirection * keyboardControled.turnSpeed * deltaT;
					
					float moveStep = keyboardControled.walkDirection * keyboardControled.walkSpeed;

					float newPlayerX = cos(keyboardControled.rotationAngle) * moveStep;
					float newPlayerY = sin(keyboardControled.rotationAngle) * moveStep;

					transform.position.x = newPlayerX;
					transform.position.y = newPlayerY;

					int paddingLeft = 10;
					int paddingTop = 10;
					int paddingRight = 50;
					int paddingBottom = 50;

					transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
					transform.position.x = transform.position.x > Game::mapWidth - paddingRight ? Game::mapWidth - paddingRight : transform.position.x;
					transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;
					transform.position.y = transform.position.y > Game::mapHeight - paddingBottom ? Game::mapHeight - paddingBottom : transform.position.y;
				}
			}*/
		}
};

#endif