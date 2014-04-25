#include "Platform.h"


Platform::Platform(void)
{
}


Platform::~Platform(void)
{
}

void Platform::init(hkpWorld *world){
	hkpRigidBodyCinfo rigidBodyInfo;	

	hkpBoxShape* plBox = new hkpBoxShape(hkVector4(2.0,0.1,2.0));
	plBox->setRadius(0.001f); // adjust convex radius to usage
	rigidBodyInfo.m_shape = plBox;  
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(plBox, 200.0f, rigidBodyInfo);

	rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
	rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	rigidBodyInfo.m_friction = 0.2f;
	rigidBodyInfo.m_restitution = 0.6f;
	rb = new hkpRigidBody(rigidBodyInfo);

	world->addEntity(rb);
	rb->removeReference();
}