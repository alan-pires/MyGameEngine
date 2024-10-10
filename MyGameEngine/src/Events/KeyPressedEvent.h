#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include <SDL.h>

class KeyPressedEvent : public Event
{
	public:
		SDL_Scancode code;
		const Uint8* keys;

		KeyPressedEvent(SDL_Scancode code, const Uint8* keys) : code(code), keys(keys) {}
};

#endif