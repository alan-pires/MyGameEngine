#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"

class CollisionEvent : public Event
{
	public:
		Entity a;
		Entity b;
		CollisionEvent(Entity a, Entity b): a(a), b(b) {}
};

#endif