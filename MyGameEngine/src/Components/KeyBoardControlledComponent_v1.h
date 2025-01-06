#ifndef KEYBOARDCONTROLLEDCOMPONENT_V1_H
#define KEYBOARDCONTROLLEDCOMPONENT_V1_H

#include <glm/glm.hpp>

struct KeyBoardControlledComponent_v1
{
	glm::vec2 position;
	int turnDirection;
	int walkDirection;
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;

	KeyBoardControlledComponent_v1()
	{
		this->turnDirection = 0;
		this->walkDirection = 0;
		this->rotationAngle = 0;
		this->walkSpeed = 6;
		this->turnSpeed = 45 ;
	}
};

#endif