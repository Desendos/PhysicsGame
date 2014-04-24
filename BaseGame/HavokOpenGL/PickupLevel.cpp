#include "PickupLevel.h"

PickupLevel::PickupLevel(void){

}

PickupLevel::PickupLevel(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}


PickupLevel::~PickupLevel(void)
{
}


void PickupLevel::createPlatformObjects(){

}