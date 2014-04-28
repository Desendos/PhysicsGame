#include "Sphere.h"


Sphere::Sphere(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
	pos = dir = Vector(0,0,0);
}


Sphere::~Sphere(void)
{
}

void Sphere::init(hkpWorld *world){
	hkpSphereShape* sphere = new hkpSphereShape(0.2);		// convex radius for spheres is exactly the sphere radius
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_shape = sphere;
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(sphere, 1.0f, rigidBodyInfo);
	rigidBodyInfo.m_position.set(1.0, 1.2, 0.0);
	rigidBodyInfo.m_friction = 0.4f;
	rigidBodyInfo.m_restitution = 0.2f;

	rb = new hkpRigidBody(rigidBodyInfo);
	sphere->removeReference();
	world->addEntity(rb);
	rb->removeReference();

}