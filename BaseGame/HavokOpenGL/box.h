#pragma once
#include "havokobj.h"
class box :
	public HavokObj
{
public:
	box(float sx=0.2f,float sy=0.2f,float sz=0.2f);
	~box(void);
	void init(hkpWorld* world);
	float sx;
	float sy;
	float sz;
};

