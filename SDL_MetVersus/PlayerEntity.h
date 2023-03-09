#pragma once
#include "BaseEntity.h"

class PlayerEntity : public BaseEntity
{
protected:
	Sint32 X_Velocity;
	Sint32 Y_Velocity;
public:
	void Update() override;
	
};

