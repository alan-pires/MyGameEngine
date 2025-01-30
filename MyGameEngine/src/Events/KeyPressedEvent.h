#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include <SDL.h>

class KeyPressedEvent : public Event
{
	public:
		SDL_Scancode code;
		SDL_EventType eventType;
		//SDL_Keycode code;

		//KeyPressedEvent(SDL_EventType eventType, SDL_Keycode code) : eventType(eventType), code(code) {}
		KeyPressedEvent(SDL_EventType eventType, SDL_Scancode code) : eventType(eventType), code(code) {}
};

#endif