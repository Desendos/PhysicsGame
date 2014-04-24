#pragma once
#include "level.h"
class LevelNormal :
	public Level
{
public:
	LevelNormal(void);
	LevelNormal(float x, float y, float z);
	~LevelNormal(void);
	void createPlatformObjects();
};

