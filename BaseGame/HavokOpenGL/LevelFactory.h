#include "Level.h"
#pragma once

class LevelFactory
{
public:
	LevelFactory(void);
	~LevelFactory(void);
	virtual Level* createLevel(int levelType, float x, float y, float z) = 0;
};

