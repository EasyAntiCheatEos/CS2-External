#pragma once
struct EntityData {
    uintptr_t entityController;
    uintptr_t entityPawn;
    std::string entityName;
    Math::Vector3 Pos;
    INT teamiddata;
    INT HEALTH;
    INT64 STEAMID;
};
struct LocalEntityData
{
    uintptr_t entityPawn;
    std::string entityName;
    Math::Vector3 Pos;
    INT teamiddata;
    INT HEALTH;
    INT64 STEAMID;
};
#include "../moneyhack.hpp"

#include "Entity/entitylist.h"
#include "Entity/local.h"