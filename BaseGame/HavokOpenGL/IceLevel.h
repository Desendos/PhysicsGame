#pragma once
#include "level.h"
class IceLevel :
	public Level
{
public:
	IceLevel(void);
	IceLevel(float x, float y, float z);
	~IceLevel(void);
	void createPlatformObjects();
};

