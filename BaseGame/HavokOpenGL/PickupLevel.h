#pragma once
#include "level.h"
class PickupLevel :
	public Level
{
public:
	PickupLevel(void);
	PickupLevel(float x, float y, float z);
	~PickupLevel(void);
	void createPlatformObjects();
};

