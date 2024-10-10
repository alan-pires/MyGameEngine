
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
#include <math.h>
#include "../Game/Game.h"

class KeyBoardMovementSystem_v2 : public System
{
private:
	double dt = 0.0;

public:
	KeyBoardMovementSystem_v2()
	{
		RequireComponent<KeyBoardControlledComponent_v2>();
		RequireComponent<SpriteComponent>();
		RequireComponent<TransformComponent>();
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
			auto& keyboardControled = entity.GetComponent<KeyBoardControlledComponent_v2>();
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();

			if (entity.HasTag("player"))
			{
				switch (e.code)
				{
				case SDL_SCANCODE_LEFT:
					keyboardControled.rotationAngle -= keyboardControled.rotationSpeed * dt;
					transform.rotation = keyboardControled.rotationAngle;
					break;
				case SDL_SCANCODE_RIGHT:
					keyboardControled.rotationAngle += keyboardControled.rotationSpeed * dt;
					transform.rotation = keyboardControled.rotationAngle;
					break;
				case SDL_SCANCODE_UP:
					transform.velocity.x += keyboardControled.walkSpeed * dt * cos(transform.rotation * M_PI / 180.0f) /*keyboardControled.walkSpeed **/;
					transform.velocity.y += keyboardControled.walkSpeed * dt * sin(transform.rotation * M_PI / 180.0f) /*keyboardControled.walkSpeed**/;
					break;
				case SDL_SCANCODE_DOWN:
					transform.velocity.x -= keyboardControled.walkSpeed * dt * cos(transform.rotation * M_PI / 180.0f) /*keyboardControled.walkSpeed **/;
					transform.velocity.y -= keyboardControled.walkSpeed * dt * sin(transform.rotation * M_PI / 180.0f) /*keyboardControled.walkSpeed**/;
					break;
				}

				//friction
				//if (!e.keys[SDL_SCANCODE_UP] && !e.keys[SDL_SCANCODE_DOWN])
				//{
				//	float frictionEffect = transform.friction * dt;

				//	if (transform.velocity.x > 0)
				//		transform.velocity.x -= frictionEffect;
				//	else if (transform.velocity.x < 0)
				//		transform.velocity.x += frictionEffect;

				//	if (transform.velocity.y > 0)
				//		transform.velocity.y -= frictionEffect;
				//	else if (transform.velocity.y < 0)
				//		transform.velocity.y += frictionEffect;

				//	if (fabs(transform.velocity.x) < frictionEffect)
				//		transform.velocity.x = 0;
				//	if (fabs(transform.velocity.y) < frictionEffect)
				//		transform.velocity.y = 0;
				//}

				transform.position.x += transform.velocity.x * dt;
				transform.position.y += transform.velocity.y * dt;

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