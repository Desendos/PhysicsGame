#pragma once
#include "platform.h"
#include "HavokObj.h"
#include "LoadTextures.h"

class Level : 
	public HavokObj
{
public:
	Level(void);
	Level(float x, float y, float z);
	~Level(void);
	float sx;
	float sy;
	float sz;
	virtual void createPlatformObjects() = 0;
	void init(hkpWorld* world);

};

