#include "Box.h"


Box::Box(void)
{
}


Box::~Box(void)
{
}

Box::Box(float x, float y, float z): HavokObj(){
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}

void Box::init(hkpWorld *world){
	float calcMass = density * sx * sy * sz;	//mass = density*vol
	hkpBoxShape* sBox = new hkpBoxShape(hkVector4(sx, sy, sz));
	sBox->setRadius(0.001f); // adjust the convex radius as req’d
	setRigidBodyInfo(world, sBox, calcMass);
}

void Box::initFixed(hkpWorld *world){
	float calcMass = density * sx * sy * sz;	//mass = density*vol
	hkpBoxShape* sBox = new hkpBoxShape(hkVector4(sx, sy, sz));
	sBox->setRadius(0.001f); // adjust the convex radius as req’d
	setRigidBodyInfoFixed(world, sBox, calcMass);
}