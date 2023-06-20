#pragma once
#include "BaseEntity.h"

class BaseProjectile : public BaseEntity
{
private:
public:
	BaseProjectile(float x, float y, String className) : BaseEntity(x, y, className, EntityType::PROJECTILE)
	{
	}
};

