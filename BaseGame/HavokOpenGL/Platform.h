#pragma once
#include "havokobj.h"
class Platform :
	public HavokObj
{
public:
	Platform(void);
	~Platform(void);

	void init(hkpWorld* world);
};

