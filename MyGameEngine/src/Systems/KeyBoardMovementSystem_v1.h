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
#include <SDL.h>
#include <math.h>

class KeyBoardMovementSystem_v1 : public System
{
	private:
		double dt = 0.0;
		
		// Estado anterior das teclas (para detectar mudanças)
		bool prevKeyUp = false;
		bool prevKeyDown = false;
		bool prevKeyLeft = false;
		bool prevKeyRight = false;

	public:
		KeyBoardMovementSystem_v1()
		{
			RequireComponent<KeyBoardControlledComponent_v1>();
			RequireComponent<TransformComponent>();
		}
		~KeyBoardMovementSystem_v1() = default;

		void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
		{
			// Não precisa mais de eventos do Game, o sistema rastreia estado localmente
		}

		void UpdateKeyboardState()
		{
			const Uint8* keys = SDL_GetKeyboardState(NULL);

			bool currentKeyUp = keys[SDL_SCANCODE_UP] != 0;
			bool currentKeyDown = keys[SDL_SCANCODE_DOWN] != 0;
			bool currentKeyLeft = keys[SDL_SCANCODE_LEFT] != 0;
			bool currentKeyRight = keys[SDL_SCANCODE_RIGHT] != 0;

			// Detectar mudanças e atualizar componentes
			for (auto& entity : GetSystemEntities())
			{
				auto& keyboard = entity.GetComponent<KeyBoardControlledComponent_v1>();

				if (currentKeyUp && !prevKeyUp) keyboard.upPressed = true;
				else if (!currentKeyUp && prevKeyUp) keyboard.upPressed = false;
				
				if (currentKeyDown && !prevKeyDown) keyboard.downPressed = true;
				else if (!currentKeyDown && prevKeyDown) keyboard.downPressed = false;

				if (currentKeyLeft && !prevKeyLeft) keyboard.leftPressed = true;
				else if (!currentKeyLeft && prevKeyLeft) keyboard.leftPressed = false;

				if (currentKeyRight && !prevKeyRight) keyboard.rightPressed = true;
				else if (!currentKeyRight && prevKeyRight) keyboard.rightPressed = false;
			}

			// Guardar estado atual para próximo frame
			prevKeyUp = currentKeyUp;
			prevKeyDown = currentKeyDown;
			prevKeyLeft = currentKeyLeft;
			prevKeyRight = currentKeyRight;
		}

		void Update(double deltaT, std::unique_ptr<EventManager>& eventManager)
		{
			UpdateKeyboardState();

			for (auto& entity : GetSystemEntities())
			{
				auto& keyboard = entity.GetComponent<KeyBoardControlledComponent_v1>();
				auto& transform = entity.GetComponent<TransformComponent>();
				
				// Calcular direção do input
				glm::vec2 inputDir(0.0f, 0.0f);
				
				if (keyboard.upPressed) inputDir.y -= 1.0f;
				if (keyboard.downPressed) inputDir.y += 1.0f;
				if (keyboard.leftPressed) inputDir.x -= 1.0f;
				if (keyboard.rightPressed) inputDir.x += 1.0f;
				
				// Normalizar diagonal (evita movimento mais rápido na diagonal)
				if (glm::length(inputDir) > 0.0f)
				{
					inputDir = glm::normalize(inputDir);
					keyboard.inputDirection = inputDir;
					
					// Acelerar na direção do input
					transform.velocity += inputDir * (float)(keyboard.acceleration * deltaT);
					
					// Limitar velocidade máxima
					if (glm::length(transform.velocity) > keyboard.walkSpeed)
					{
						transform.velocity = glm::normalize(transform.velocity) * keyboard.walkSpeed;
					}
					
					// Atualizar rotação para onde está movendo (em graus)
					transform.rotation = atan2(inputDir.y, inputDir.x) * 180.0 / M_PI;
				}
				else
				{
					// Aplicar fricção quando não há input
					float currentSpeed = glm::length(transform.velocity);
					if (currentSpeed > 0.0f)
					{
						float newSpeed = std::max(0.0f, currentSpeed - (float)(keyboard.friction * deltaT));
						if (newSpeed > 0.0f)
						{
							transform.velocity = glm::normalize(transform.velocity) * newSpeed;
						}
						else
						{
							transform.velocity = glm::vec2(0.0f, 0.0f);
						}
					}
				}
				
				// Aplicar movimento
				transform.position += transform.velocity * (float)deltaT;
				
				// Bounds checking
				const float paddingLeft = 10.0f;
				const float paddingTop = 10.0f;
				const float paddingRight = 50.0f;
				const float paddingBottom = 50.0f;
				
				if (transform.position.x < paddingLeft) {
					transform.position.x = paddingLeft;
					transform.velocity.x = 0.0f;
				}
				else if (transform.position.x > Game::mapWidth - paddingRight) {
					transform.position.x = Game::mapWidth - paddingRight;
					transform.velocity.x = 0.0f;
				}
				
				if (transform.position.y < paddingTop) {
					transform.position.y = paddingTop;
					transform.velocity.y = 0.0f;
				}
				else if (transform.position.y > Game::mapHeight - paddingBottom) {
					transform.position.y = Game::mapHeight - paddingBottom;
					transform.velocity.y = 0.0f;
				}
			}
		}
};

#endif