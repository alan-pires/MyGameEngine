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
	private:
		double dt = 0.0;

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
		}

		void onKeyPressedEvent(KeyPressedEvent& e)
		{
			for (auto& entity: GetSystemEntities())
			{
				auto& keyboardControled = entity.GetComponent<KeyBoardControlledComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();

				if (entity.HasTag("player"))
				{
					switch (e.code)
					{
					case SDL_SCANCODE_LEFT:
						keyboardControled.rotationAngle -= keyboardControled.rotationSpeed;
						transform.rotation = keyboardControled.rotationAngle;				
						break;
					case SDL_SCANCODE_RIGHT:
						keyboardControled.rotationAngle += keyboardControled.rotationSpeed;
						transform.rotation = keyboardControled.rotationAngle;
						break;
					case SDL_SCANCODE_UP:
						transform.position.x += keyboardControled.walkSpeed * cos(keyboardControled.rotationAngle * M_PI / 180.0f) * dt;
						transform.position.y += keyboardControled.walkSpeed * sin(keyboardControled.rotationAngle * M_PI / 180.0f) * dt;
						break;
					case SDL_SCANCODE_DOWN:
						transform.position.x -= keyboardControled.walkSpeed * cos(keyboardControled.rotationAngle * M_PI / 180.0f) * dt;
						transform.position.y -= keyboardControled.walkSpeed * sin(keyboardControled.rotationAngle * M_PI / 180.0f) * dt;
						break;
					}
				}

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


		void Update(double deltaT)
		{
			dt = deltaT;
		}
};

#endif