#pragma once

#include <optional>
#include <string>

#include "EntityContext.h"
#include "Ref.h"

class EntityRegistry : public EntityRegistryBase, public std::enable_shared_from_this<EntityRegistry> {
public:
	std::string name;
	entt::basic_registry<uint32_t> ownedRegistry;
};

struct EntityRegistryRefTraits : SharePtrRefTraits<EntityRegistry> {};
class WeakStorageEntity {
public:
	enum class EmptyInit : int {
		NoValue = 0,
	};

	enum class VariadicInit : int {
		NonAmbiguous = 0,
	};

	WeakRefT<EntityRegistryRefTraits> registry;
	uint32_t entity;
};

class OwnerStorageEntity {
public:
	enum class EmptyInit : int {
		NoValue = 0x0,
	};

	enum class VariadicInit : int {
		NonAmbiguous = 0x0,
	};

	std::optional<EntityContext> context;
};

class StackResultStorageEntity {
public:
	std::optional<EntityContext> context;
};

struct EntityRefTraits {
public:
	using StackRef = EntityContext;
	using OwnerStackRef = StackRef;
	using WeakStorage = WeakStorageEntity;
	using OwnerStorage = OwnerStorageEntity;
	using StackResultStorage = StackResultStorageEntity;
};

class WeakEntityRef : public WeakRefT<struct EntityRefTraits> {};
