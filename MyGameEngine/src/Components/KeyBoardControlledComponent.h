#ifndef KEYBOARDCONTROLLEDCOMPONENT_H
#define KEYBOARDCONTROLLEDCOMPONENT_H

#include <glm/glm.hpp>

struct KeyBoardControlledComponent
{
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for back, +1 for front
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;

	KeyBoardControlledComponent(int turnDirection = 0, int walkDirection = 0, float rotationAngle = 0.0, float walkSpeed = 0.0, float turnSpeed = 0.0)
	{
		this->turnDirection = turnDirection;
		this->walkDirection = walkDirection;
		this->rotationAngle = rotationAngle;
		this->walkSpeed = walkSpeed;
		this->turnSpeed = turnSpeed;
	}
};

#endif