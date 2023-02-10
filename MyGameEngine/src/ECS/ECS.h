#ifndef ESC_H
#define ESC_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <map>
#include <typeindex>
#include <set>
#include <memory>
#include <algorithm>
#include <deque>
#include "../Logger/Logger.h"
#include <string>
using namespace std;

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

// ============================== ENTITY CLASS ====================================
class Entity
{
	private:
		int id;

	public:
		Entity(int id): id(id){};
		Entity(const Entity& entity) = default;
		void Kill();
		int GetId() const;

		// Manage entity tags and groups
		void setTag(const std::string& tag);
		bool HasTag(const std::string& tag) const;
		void setGroup(const std::string& group);
		bool BelongsToGroup(const std::string& group) const;

		bool operator ==(const Entity& other) const { return id == other.id; }
        bool operator !=(const Entity& other) const { return id != other.id; }
        bool operator >(const Entity& other) const { return id > other.id; }
        bool operator <(const Entity& other) const { return id < other.id; }

		template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
		template <typename TComponent> void RemoveComponent();
		template <typename TComponent> bool HasComponent() const;
		template <typename TComponent> TComponent& GetComponent() const;
		class Registry* registry;
};

// ============================== COMPONENT CLASSES ====================================
// Interface for classes that inhenrits from it
struct IComponent
{
	protected:
		static int nextId;
};

// used to assign a unique id to a component type
template <typename T>
class Component: public IComponent
{
	public:
		//returns the unique id of the component<T>
		static int GetId()
		{
			static auto id = nextId++;
			return id;
		}
};

// ============================== SYSTEM CLASS ====================================
class System
{
	private:
		Signature componentSignature;
		std::vector<Entity> entities;

	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityToSystem(Entity entity);
		std::vector<Entity> GetSystemEntities() const;
		const Signature& GetComponetSignature() const;

		template <typename TComponent> void RequireComponent();
};

// ============================== POOL CLASSES ====================================
class IPool
{
	public:
		virtual ~IPool() = default;
		virtual void RemoveEntityFromPool(int entityId) = 0;
};

template <typename T>
class Pool: public IPool{
	private:
		std::vector<T> data;
		int size;
		// Helper maps to keep track of entity ids per index. So the vector (data) is always packed
		std::unordered_map<int, int> entityIdToIndex;
		std::unordered_map<int, int> indexToEnityId;
	
	public:
		Pool(int capacity = 100)
		{
			size = 0;
			data.resize(capacity);
		}
		virtual ~Pool() = default;

		bool isEmpty() const
		{
			return size == 0;
		}

		int GetSize() const
		{
			return size;
		}

		void Resize(int n)
		{
			data.resize(n);
		}

		void Clear()
		{
			data.clear();
			size = 0;
		}

		void Add(T object)
		{
			data.push_back(object);
		}

		void Set(int entityId, T object)
		{
			// if the element already exists, replace the component object
			if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
			{
				int index = entityIdToIndex[entityId];
				data[index] = object;
			}
			else 
			{
				// when adding a new object, keep track of the entity ids and their vector index
				int index = size;
				entityIdToIndex.emplace(entityId, index);
				indexToEnityId.emplace(index, entityId);
				if (index >= data.capacity())
					data.resize(size * 2);
				data[index] = object;
				size++;
			}			
		}

		void Remove(int entityId)
		{
			// copy the last element to the deleted position to keep the array packed
			int intdexOfRemoved = entityIdToIndex[entityId];
			int indexOfLast = size - 1;
			data[intdexOfRemoved] = data[indexOfLast];
			// update the index-entity maps to point to the correct elements
			int entityIdOfLastElement = indexToEnityId[indexOfLast];
			entityIdToIndex[entityIdOfLastElement] = intdexOfRemoved;
			indexToEnityId[intdexOfRemoved] = entityIdOfLastElement;

			entityIdToIndex.erase(entityId);
			indexToEnityId.erase(indexOfLast);
			size--;
		}

		void RemoveEntityFromPool(int entityId) override
		{
			if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
				Remove(entityId);
		}

		T& Get(int entityId)
		{
			int index = entityIdToIndex[entityId];
			return static_cast<T&>(data[index]);
		}

		T& operator [](unsigned int index)
		{
			return data[index];
		}
};

// ============================== REGISTRY (MANAGER) CLASS ====================================
class Registry
{
	private:
		int numEntities = 0;

		// Vector of components pools, each pool constains all the data for a certain component type
		// Vector index = component type id, Pool index = entity id
		std::vector<std::shared_ptr<IPool>> componentPools;
		std::vector<Signature> entityComponentSignatures;
		// Map of active systems
		std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
		// Set of entities that are flagged to be added or removed in the nexet registry Update
		std::set<Entity> entitiesToBeAdded;
		std::set<Entity> entitiesToBeKilled;
		// Entity tags (onetag name per entity)
		std::unordered_map<std::string, Entity> entityPerTag;
		std::unordered_map<int, std::string> tagPerEntity;
		// Entity groups ( a set of entities per group name)
		std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
		std::unordered_map<int, std::string> groupPerEntity;

		// list of free entities ids that were previously removed
		std::deque<int> freeIds;

	public:
		Registry() = default;
		void Update();

		//Entity Management
		Entity CreateEntity();
		void KillEntity(Entity entity);
		void AddEntityToSystems(Entity entity);
		void RemoveEntityFromSystems(Entity entity);

		// Tag management
		void TagEntity(Entity entity, const std::string& tag);
		bool EntityHasTag(Entity entity, const std::string& tag) const;
		Entity GetEntityByTag(const std::string& tag) const;
		void RemoveEntityTag(Entity entity);

		// Group Management
		void GroupEntity(Entity entity, const std::string& group);
		bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
		std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
		void RemoveEntityGroup(Entity entity);

		//Componenent Management
		template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		template <typename TComponent> void RemoveComponent(Entity entity);
		template <typename TComponent> bool HasComponent(Entity entity) const;
		template <typename TComponent> TComponent& GetComponent(Entity entity) const;

		//System Management
		template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
		template <typename TSystem> void RemoveSystem();
		template <typename TSystem> bool HasSystem() const;
		template <typename TSystem> TSystem& GetSystem() const;

		// // Checks the component signature of an entity and add the entity to the systems that are interested in it
		// void AddEntityToSystems(Entity entity);
};

// ======================== TEMPLATE FUNCTIONS ==============================

// ======================== COMPONENT TEMPLATE FUNCTIONS ========================
template <typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= componentPools.size())
		componentPools.resize(componentId + 1, nullptr);

	if (!componentPools[componentId])
	{
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}
	std::shared_ptr<Pool<TComponent>> currentComponentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

 	TComponent newComponent(std::forward<TArgs>(args)...);
	currentComponentPool->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);
	// Logger::Log("Component ID: " + std::to_string(componentId) + " was added to the Entity ID: " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	// Remove the component from the component list for that entity
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	componentPool->Remove(entityId);
	entityComponentSignatures[entityId].set(componentId, false);
	Logger::Log("Component ID: " + std::to_string(componentId) + " was removed from the Entity ID: " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool->Get(entityId);
}

// ======================== TEMPLATE SYSTEM FUNCTIONS ========================

// Creates a new System Object and adds it to the unordered map Systems
template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args)
{
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem()
{
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const
{
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const
{
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

// ======================== TEMPLATE ENTITY FUNCTIONS ========================

template <typename TComponent, typename ...TArgs> 
void Entity::AddComponent(TArgs&& ...args)
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent> 
void Entity::RemoveComponent()
{
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const
{
	return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{
	return registry->GetComponent<TComponent>(*this);
}

#endif