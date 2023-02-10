#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include "../Components/KeyBoardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"

class KeyBoardMovementSystem : public System
{
	public:
		KeyBoardMovementSystem()
		{
			RequireComponent<KeyBoardControlledComponent>();
			RequireComponent<RigidBodyComponent>();
			RequireComponent<SpriteComponent>();
		}
		~KeyBoardMovementSystem() = default;

		void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
		{
			eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyBoardMovementSystem::onKeyPressedEvent);
			//eventManager->SubscribeToEvent<KeyReleasedEvent>(this, &KeyBoardMovementSystem::onKeyReleased);
		}

		void onKeyPressedEvent(KeyPressedEvent& e)
		{
			for (auto entity: GetSystemEntities())
			{
				const auto keyboardControl = entity.GetComponent<KeyBoardControlledComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

				switch (e.symbol)
				{
					case SDLK_UP:
						rigidBody.velocity = keyboardControl.upVelocity;
						sprite.srcRect.y = sprite.height * 0;
						break;
					case SDLK_RIGHT:
						rigidBody.velocity = keyboardControl.rightVelocity;
						sprite.srcRect.y = sprite.height * 1;
						break;
					case SDLK_DOWN:
						rigidBody.velocity = keyboardControl.downVelocity;
						sprite.srcRect.y = sprite.height * 2;
						break;
					case SDLK_LEFT:
						rigidBody.velocity = keyboardControl.leftVelocity;
						sprite.srcRect.y = sprite.height * 3;
						break;
				}

					/*switch (e.code)
					{
					case SDL_SCANCODE_UP:
						rigidBody.velocity = keyboardControl.upVelocity;
						sprite.srcRect.y = sprite.height * 0;
						break;
					case SDL_SCANCODE_RIGHT:
						rigidBody.velocity = keyboardControl.rightVelocity;
						sprite.srcRect.y = sprite.height * 1;
						break;
					case SDL_SCANCODE_DOWN:
						rigidBody.velocity = keyboardControl.downVelocity;
						sprite.srcRect.y = sprite.height * 2;
						break;
					case SDL_SCANCODE_LEFT:
						rigidBody.velocity = keyboardControl.leftVelocity;
						sprite.srcRect.y = sprite.height * 3;
						break;
					}*/
			}
		}

		void onKeyReleased(KeyReleasedEvent &e)
		{
			for (auto entity: GetSystemEntities())
			{
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
				rigidBody.velocity = glm::vec2(0.0);
			}
		}

		void Update()
		{
			
		}
};

#endif