#include "HavokObj.h"

HavokObj::HavokObj(){ 
	rb = NULL; 
	density = 2000;	//nominal value.  Note: steel approx 8000kg/cubic metre
}

HavokObj::~HavokObj(){
	delete rb;
}
void HavokObj::setRigidBodyInfo(hkpWorld *world, hkpShape *hks, float mass){
	rigidBodyInfo.m_shape = hks;  
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(hks, mass, rigidBodyInfo);
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
	rigidBodyInfo.m_position.set(pos.x, pos.y, pos.z);
	rigidBodyInfo.m_friction = 0.2f;
	rigidBodyInfo.m_restitution = 0.6f;
	rb = new hkpRigidBody(rigidBodyInfo);
	hks->removeReference();
	world->addEntity(rb);
	rb->removeReference();
}

void HavokObj::removeRigidBody(hkpWorld *world){
	world->removeEntity(rb);
	rb = NULL;
	delete rb;
}