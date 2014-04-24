#include "IceLevel.h"


IceLevel::IceLevel(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}


IceLevel::~IceLevel(void)
{
}


void IceLevel::createPlatformObjects(){

}