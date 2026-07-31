#ifndef KEYBOARDCONTROLLEDCOMPONENT_V1_H
#define KEYBOARDCONTROLLEDCOMPONENT_V1_H

#include <glm/glm.hpp>

struct KeyBoardControlledComponent_v1
{
	// Estados das teclas (true = pressionada)
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;
	
	float walkSpeed = 200.0f;        // pixels por segundo
	float acceleration = 800.0f;     // aceleração para movimento suave
	float friction = 600.0f;         // desaceleração quando solta a tecla
	
	glm::vec2 inputDirection = glm::vec2(0, 0);  // direção normalizada do input

	KeyBoardControlledComponent_v1()
	{
		// Valores padrão já inicializados acima
	}
};

#endif