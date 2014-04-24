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

void HavokObj::setRot(){
	/*rigidBodyInfo.m_rotation = rigidBodyInfo.m_rotation + vec;
	quat->*/
//	rigidBodyInfo.m_rotation.m_vec = rigidBodyInfo.m_rotation.m_vec + vec;
	//hkVector4Parameter axis();
	//hkQuaternion quat(0.9512512425641978,0.044943455527547777,0.16773125949652062,0.2548870022441788);
	//hkQuaternion quat;
	//hkVector4Parameter para(hkVector4(0,10,0,0.0f));
//	hkVector4 rot(0,10,0,0.0f);


	//rigidBodyInfo.m_rotation = quat;
	//rigidBodyInfo.m_rotation * quat;
	//hkTransform tran(quat, rot);
	//rigidBodyInfo.setTransform(tran);
	//para = rigidBodyInfo.m_rotation ;
	//rigidBodyInfo.m_rotation.set(0.9512512425641978,0.044943455527547777,0.16773125949652062,0.2548870022441788);

}