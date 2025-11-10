#pragma once

#include <cstdint>

#include "../entt/src/entt/entity/registry.hpp"

class EntityRegistryBase {
public:
	entt::basic_registry<uint32_t>& registry;
	uint32_t viewedEntity;
	bool viewUsesViewedContext;
	unsigned int id;
};

class EntityContextBase {
public:
	EntityRegistryBase& registry;
	uint32_t entity;
};

class EntityContext : public EntityContextBase { };
