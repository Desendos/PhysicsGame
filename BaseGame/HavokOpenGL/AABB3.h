/////////////////////////////////////////////////////////////////////////////
//
// Based on 3D Math Primer for Games and Graphics Development
// AABB3.h - Declarations for class AABB3
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __AABB3_H_INCLUDED__
#define __AABB3_H_INCLUDED__

#include "Vector.h" //****Amended by SM.
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

class Matrix4x3;

//---------------------------------------------------------------------------
// class AABB3
// Implement a 3D axially aligned bounding box

class AABB3 {
public:
	Vector	min, max;// Min and max values.  

	AABB3();  //****added by SM.
	Vector	size() const { return max - min; }  // Query for dimensions
	float	xSize() { return max.x - min.x; }
	float	ySize() { return max.y - min.y; }
	float	zSize() { return max.z - min.z; }
	Vector	centre() const { return (min + max) * .5f; }

	// Fetch one of the eight corner points.  See the
	// .cpp for numbering conventions
	Vector	corner(int i) const;

// Box operations
	void empty(); // "Empty" the box, by setting the values to really large/small numbers
	void add(const Vector &p); // Add a point to the box
	void	add(const AABB3 &box);  // Add an AABB to the box
	void	setToTransformedBox(const AABB3 &box, const Matrix4x3 &m); // Transform the box and compute the new AABB

// Containment/intersection tests
	bool isEmpty() const;  // Return true if the box is enmpty
	bool contains(const Vector &p) const;  // Return true if the box contains a point
	Vector	closestPointTo(const Vector &p) const;  // Return the closest point on this box to another point
	bool	intersectsSphere(const Vector &center, float radius) const;  // Return true if we intersect a sphere

	// Parametric intersection with a ray.  Amended by SM.
	bool rayIntersect(Vector rayOrg, Vector rayEnd, Vector aabbPos);

	// Classify box as being on one side or the other of a plane
	int	classifyPlane(const Vector &n, float d) const;

	// Dynamic intersection with plane
	float	intersectPlane(const Vector &n, float planeD, const Vector &dir) const;

	void AABB3::drawEdge(int v0, int v1);  //****added by SM.
	void render();
	static bool intersectAABBs(AABB3 box1, const Vector pos1, AABB3 box2, const Vector pos2, AABB3 *boxIntersect = 0);	//**** amended to include positions. SM.
};

// Check if two AABBs intersect, and return true if so.  Optionally return
// the AABB of their intersection if an intersection is detected

//bool intersectAABBs(const AABB3 &box1, const AABB3 &box2, AABB3 *boxIntersect = 0);

// Return parametric point in time when a moving AABB collides
// with a stationary AABB.  Returns > 1 if no intersection
float	intersectMovingAABB(const AABB3 &stationaryBox,	const AABB3 &movingBox,	const Vector &d);

/////////////////////////////////////////////////////////////////////////////
#endif // #ifndef __AABB3_H_INCLUDED__
