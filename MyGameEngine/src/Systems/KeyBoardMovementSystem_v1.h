#ifndef KEYBOARDCMOVEMENTSYSTEM_V1_H
#define KEYBOARDCMOVEMENTSYSTEM_V1_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include "../Components/KeyBoardControlledComponent_v1.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include <math.h>

class KeyBoardMovementSystem_v1 : public System
{
	private:
		double dt = 0.0;

	public:
		KeyBoardMovementSystem_v1()
		{
			RequireComponent<KeyBoardControlledComponent_v1>();
			RequireComponent<TransformComponent>();
		}
		~KeyBoardMovementSystem_v1() = default;

		void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
		{
			eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyBoardMovementSystem_v1::onKeyPressedEvent);
		}

		void onKeyPressedEvent(KeyPressedEvent& e)
		{
			for (auto& entity: GetSystemEntities())
			{
				auto& keyboardControled = entity.GetComponent<KeyBoardControlledComponent_v1>();
				auto& transform = entity.GetComponent<TransformComponent>();

				switch (e.eventType)
				{
				case SDL_KEYDOWN:
					if (e.code == SDLK_UP)
						keyboardControled.walkDirection = +1;
					if (e.code == SDLK_DOWN)
						keyboardControled.walkDirection = -1;
					if (e.code == SDLK_RIGHT)
						keyboardControled.turnDirection = +1;
					if (e.code == SDLK_LEFT)
						keyboardControled.turnDirection = -1;
					break;
				case SDL_KEYUP:
					if (e.code == SDLK_UP || e.code == SDLK_DOWN)
						keyboardControled.walkDirection = 0;
					if (e.code == SDLK_RIGHT || e.code == SDLK_LEFT)
						keyboardControled.turnDirection = 0;
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
			for (auto& entity : GetSystemEntities())
			{
				auto& player = entity.GetComponent<KeyBoardControlledComponent_v1>();
				auto& transform = entity.GetComponent<TransformComponent>();

				transform.rotation += player.turnDirection * player.turnSpeed * deltaT;
				
				float moveStep = player.walkDirection * player.walkSpeed;

				transform.position.x += cos(transform.rotation) * moveStep;
				transform.position.y += sin(transform.rotation) * moveStep;
				/*transform.position.x += cos(transform.rotation * M_PI / 180.0f) * moveStep;
				transform.position.y += sin(transform.rotation * M_PI / 180.0f) * moveStep;*/
			}
		}
};

#endif