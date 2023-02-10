#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <list>

class IEventCallback
{
	private:
		virtual void Call(Event& e) = 0;

	public:
		virtual ~IEventCallback() = default;

		void Execute(Event& e)
		{
			Call(e);
		}
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
	private:
		typedef void (TOwner::*CallbackFunction)(TEvent&);

		TOwner *owner;
		CallbackFunction callbackFunction;

		virtual void Call(Event& e) override
		{
			std::invoke(callbackFunction, owner, static_cast<TEvent&>(e));
		}
	
	public:
		EventCallback(TOwner* owner, CallbackFunction callbackFunction)
		{
			this->owner = owner;
			this->callbackFunction = callbackFunction;
		}

		virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventManager
{
	private:
		std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

	public:
		EventManager() {}
		~EventManager() {}
	
	void Reset()
	{
		subscribers.clear();
	}
	
	// Register function pointers (Callbacks) that'll be listening for events and waiting to be called
	template <typename TEvent, typename TOwner>
	void SubscribeToEvent(TOwner *owner, void (TOwner::*callbackFunction)(TEvent&))
	{
		if (!subscribers[typeid(TEvent)].get())
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();

		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(owner, callbackFunction);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}
	
	// 
	template <typename TEvent, typename ...TArgs>
	void emitEvent(TArgs&& ...args)
	{
		auto handlers = subscribers[typeid(TEvent)].get();
		if (handlers)
		{
			for (auto it = handlers->begin(); it != handlers->end(); it++)
			{
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->Execute(event);
			}
		}
	}
};

#endif