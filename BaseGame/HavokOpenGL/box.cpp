#include "box.h"


box::box(float x, float y, float z): HavokObj(){
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}

box::~box(void)
{
}


void box::init(hkpWorld *world){
	float calcMass = density * sx * sy * sz;	//mass = density*vol
	hkpBoxShape* sBox = new hkpBoxShape(hkVector4(sx, sy, sz));
	sBox->setRadius(0.001f); // adjust the convex radius as req’d
	setRigidBodyInfo(world, sBox, calcMass);
}