#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;
	glm::vec2 velocity;
	double acceleration;
	double friction;

	TransformComponent(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0, glm::vec2 velocity = glm::vec2(0, 0), double acceleration = 0.0, double friction = 0.0)
	{
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->friction = friction;
	}
};

#endif