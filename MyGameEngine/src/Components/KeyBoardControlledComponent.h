#ifndef KEYBOARDCONTROLLEDCOMPONENT_H
#define KEYBOARDCONTROLLEDCOMPONENT_H

#include <glm/glm.hpp>

struct KeyBoardControlledComponent
{
	float rotationAngle;
	float walkSpeed;
	float rotationSpeed;

	KeyBoardControlledComponent(float rotationAngle = 0.0, float walkSpeed = 0.0, float rotationSpeed = 0.0)
	{
		this->rotationAngle = rotationAngle;
		this->walkSpeed = walkSpeed;
		this->rotationSpeed = rotationSpeed;
	}
};

#endif