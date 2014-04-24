#pragma once
#include "level.h"
class EnemyLevel :
	public Level
{
public:
	EnemyLevel(void);
	EnemyLevel(float x, float y, float z);
	~EnemyLevel(void);
	void createPlatformObjects();
};

