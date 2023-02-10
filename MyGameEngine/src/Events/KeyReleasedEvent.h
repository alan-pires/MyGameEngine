#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include <SDL.h>

class KeyReleasedEvent : public Event
{
	public:
		//SDL_Scancode code;
		SDL_Keycode symbol;

		KeyReleasedEvent(SDL_Keycode symbol): symbol(symbol){}
		//KeyReleasedEvent(SDL_Scancode code) : code(code) {}
};

#endif