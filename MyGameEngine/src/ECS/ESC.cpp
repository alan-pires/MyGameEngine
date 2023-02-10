#include "ECS.h"

int IComponent::nextId = 0;

int Entity::GetId() const
{
	return id;
}

void Entity::Kill()
{
	registry->KillEntity(*this);
}

void Entity::setTag(const std::string& tag)
{
	registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const
{
	return registry->EntityHasTag(*this, tag);
}

void Entity::setGroup(const std::string& group)
{
	registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group) const
{
	return registry->EntityBelongsToGroup(*this, group);
}

void System::AddEntityToSystem(Entity entity)
{
	// System::GetSystemEntities().push_back(entity);
	entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other){
		 return entity == other;
	 }), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return entities;
}

const Signature& System::GetComponetSignature() const
{
	return componentSignature;
}

Entity Registry::CreateEntity()
{
	int entityId;

	if (freeIds.empty())
	{
		entityId = numEntities++;
		if (entityId >= entityComponentSignatures.size())
			entityComponentSignatures.resize(entityId + 1);
	}
	else
	{
		entityId = freeIds.front();
		freeIds.pop_front();
	}
	Entity entity(entityId);	
	entity.registry = this;
	entitiesToBeAdded.insert(entity);
	// Logger::Log("Entity created whit id " + std::to_string(entityId));
	return entity;
}

void Registry::KillEntity(Entity entity)
{
	entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity)
{
	const auto entityId = entity.GetId();
	const auto& entityComponentSignature = entityComponentSignatures[entityId];
	for (auto& system: systems)
	{
		const auto& systemComponentSignature = system.second->GetComponetSignature();
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested)
			system.second->AddEntityToSystem(entity);
	}
}

void Registry::RemoveEntityFromSystems(Entity entity)
{
	for (auto system : systems)
	{
		system.second->RemoveEntityToSystem(entity);
		entityComponentSignatures[entity.GetId()].reset();
		freeIds.push_back(entity.GetId());
	}
}

void Registry::TagEntity(Entity entity, const std::string& tag)
{
	entityPerTag.emplace(tag, entity);
	tagPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const
{
	if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end())
		return false;
	return entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string& tag) const
{
	return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity)
{
	auto taggedEntity = tagPerEntity.find(entity.GetId());
	if (taggedEntity != tagPerEntity.end())
	{
		auto tag = taggedEntity->second;
		entityPerTag.erase(tag);
		tagPerEntity.erase(taggedEntity);
	}
}


void Registry::GroupEntity(Entity entity, const std::string& group)
{
	entitiesPerGroup.emplace(group, std::set<Entity>());
	entitiesPerGroup[group].emplace(entity);
	groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const
{
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
		return false;
	}
	auto groupEntities = entitiesPerGroup.at(group);
	return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const
{
	auto& setOfEntities = entitiesPerGroup.at(group);
	return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity)
{
	auto groupedEntity = groupPerEntity.find(entity.GetId());
	if (groupedEntity != groupPerEntity.end())
	{
		auto group = entitiesPerGroup.find(groupedEntity->second);
		if (group != entitiesPerGroup.end())
		{
			auto entityInGroup = group->second.find(entity);
			if (entityInGroup != group->second.end())
				group->second.erase(entityInGroup);
		}
		groupPerEntity.erase(groupedEntity);
	}
}

void Registry::Update()
{
	// Add the entities that are waiting to be created to the active systems
	for (auto entity: entitiesToBeAdded)
	{
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();

	for (auto entity: entitiesToBeKilled)
	{
		RemoveEntityFromSystems(entity);
		entityComponentSignatures[entity.GetId()].reset();

		// Remove the entity from the component pools
		for (auto pool : componentPools)
		{
			if (pool)
				pool->RemoveEntityFromPool(entity.GetId());
		}

		// Make the entity Id available to be reused
		freeIds.push_back(entity.GetId());

		RemoveEntityTag(entity);
		RemoveEntityGroup(entity);
	}
	entitiesToBeKilled.clear();
}