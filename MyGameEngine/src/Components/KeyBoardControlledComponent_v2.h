#ifndef KEYBOARDCONTROLLEDCOMPONENT_V2_H
#define KEYBOARDCONTROLLEDCOMPONENT_V2_H

#include <glm/glm.hpp>
#include "../Utils/Enum.h"

struct KeyBoardControlledComponent_v2
{
	glm::vec2 position;
	int turnDirection;
	int walkDirection;
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	playerDirection direction;

	KeyBoardControlledComponent_v2()
	{
		this->turnDirection = 0;
		this->walkDirection = 0;
		this->rotationAngle = 0;
		this->walkSpeed = 250;
		this->turnSpeed = 45;
		this->direction = playerDirection::Up;
	}
};

#endif