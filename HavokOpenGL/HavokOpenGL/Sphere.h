#pragma once
#include "havokobj.h"
class Sphere :
	public HavokObj
{
public:
	Sphere(float x, float y, float z);
	~Sphere(void);
	void init(hkpWorld* world);
	float sx;
	float sy;
	float sz;
};

