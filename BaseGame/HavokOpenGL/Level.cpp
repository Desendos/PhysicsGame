#include "Level.h"

Level::Level(void){
	pos = dir = Vector(0,0,0);
}

Level::Level(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}


Level::~Level(void)
{
}

void Level::init(hkpWorld *world){
	float calcMass = density * sx * sy * sz;	//mass = density*vol
	hkpBoxShape* sBox = new hkpBoxShape(hkVector4(sx, sy, sz));
	sBox->setRadius(0.001f); // adjust the convex radius as req’d
	setRigidBodyInfo(world, sBox, calcMass);
}
