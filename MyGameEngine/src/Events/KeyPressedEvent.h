#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "../EventManager/EventManager.h"
#include <SDL.h>

class KeyPressedEvent : public Event
{
	public:
		SDL_Scancode code;
		//SDL_Keycode symbol;

		//KeyPressedEvent(SDL_Keycode symbol): symbol(symbol){}
		KeyPressedEvent(SDL_Scancode code) : code(code) {}
};

#endif