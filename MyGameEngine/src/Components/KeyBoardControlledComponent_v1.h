#ifndef KEYBOARDCONTROLLEDCOMPONENT_V1_H
#define KEYBOARDCONTROLLEDCOMPONENT_V1_H

#include <glm/glm.hpp>

struct KeyBoardControlledComponent_v1
{
	float rotationAngle;
	float walkSpeed;
	float rotationSpeed;

	KeyBoardControlledComponent_v1(float rotationAngle = 0.0, float walkSpeed = 0.0, float rotationSpeed = 0.0)
	{
		this->rotationAngle = rotationAngle;
		this->walkSpeed = walkSpeed;
		this->rotationSpeed = rotationSpeed;
	}
};

#endif