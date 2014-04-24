/////////////////////////////////////////////////////////////////////////////
// Based on 3D Math Primer for Games and Graphics Development
/////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdlib.h>
#include "AABB3.h"
//#include "Matrix4x3.h"
//#include "CommonStuff.h"

AABB3::AABB3(){  //****added by SM.
	empty();
}

//---------------------------------------------------------------------------
// AABB3::corner
// Return one of the 8 corner points.  The points are numbered as follows:
//      6---- 7              +Y
//     /|   / |              |
//    / 4--/--5              |  
//   2--/-3   /              ----------- +X
//   | /  |  /              /
//   |/   | /              / 
//   0----1              +Z
//
// Bit 0 selects min.x vs. max.x
// Bit 1 selects min.y vs. max.y
// Bit 2 selects min.z vs. max.z

Vector	AABB3::corner(int i) const {
	assert(i >= 0);  // Make sure index is in range...
	assert(i <= 7);

//	return Vector( (i & 1) ? max.x : min.x, (i & 2) ? max.y : min.y, (i & 4) ? max.z : min.z );
	return Vector( (i & 1) ? max.x : min.x, (i & 2) ? max.y : min.y, (i & 4) ? min.z : max.z ); //****when +z as above diag. SM.
}

//---------------------------------------------------------------------------
// AABB3::empty
// "Empty" the box, by setting the values to really large/small numbers
void AABB3::empty() {
	const float kBigNumber = 1e37f;
	min.x = min.y = min.z = kBigNumber;
	max.x = max.y = max.z = -kBigNumber;
}

//---------------------------------------------------------------------------
// AABB3::add
// Add a point to the AABB
void AABB3::add(const Vector &p) {
	if (p.x < min.x) min.x = p.x;  // Expand the box as necessary to contain the point.
	if (p.x > max.x) max.x = p.x;
	if (p.y < min.y) min.y = p.y;  //****y and z not correct
	if (p.y > max.y) max.y = p.y;
	if (p.z < min.z) min.z = p.z;
	if (p.z > max.z) max.z = p.z;
}

//---------------------------------------------------------------------------
// AABB3::add
// Add an AABB to the box
void AABB3::add(const AABB3 &box) {
	if (box.min.x < min.x) min.x = box.min.x;  // Expand the box as necessary.
	if (box.min.x > max.x) max.x = box.min.x;
	if (box.min.y < min.y) min.y = box.min.y; //****
	if (box.min.y > max.y) max.y = box.min.y;
	if (box.min.z < min.z) min.z = box.min.z;
	if (box.min.z > max.z) max.z = box.min.z;
}
//****Added by SM for rendering
void AABB3::drawEdge(int v0, int v1){
	Vector p1 = corner(v0); Vector p2 = corner(v1);
	glColor3f(1.0f,1.0f, 0.0f);  // Draw edges in spec colour
	glBegin(GL_LINES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
}

void AABB3::render(){
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f,1.0f, 0.0f, 1.0f);  // Draw edges in spec colour
	glLineWidth(1);
	glPushMatrix();
		drawEdge(0,1);
		drawEdge(0,2);
		drawEdge(2,3);
		drawEdge(1,3);
		drawEdge(2,6);
		drawEdge(1,5);
		drawEdge(6,7);
		drawEdge(7,3);
		drawEdge(4,5);
		drawEdge(0,4);
		drawEdge(4,6);
		drawEdge(5,7);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}
///////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// AABB3::isEmpty
// Return true if the box is empty

bool AABB3::isEmpty() const {
	return (min.x > max.x) || (min.y > max.y) || (min.z > max.z);  // Check if we're inverted on any axis
}

//---------------------------------------------------------------------------
// AABB3::contains
// Return true if the box contains a point
bool AABB3::contains(const Vector &p) const {
	// Check for overlap on each axis
	return (p.x >= min.x) && (p.x <= max.x) && (p.y >= min.y) && (p.y <= max.y) && (p.z >= min.z) && (p.z <= max.z);
}

//---------------------------------------------------------------------------
// AABB3::closestPointTo
// Return the closest point on this box to another point
Vector AABB3::closestPointTo(const Vector &p) const {
	Vector r;  // "Push" p into the box, on each dimension
	if (p.x < min.x) {
		r.x = min.x;
	} else if (p.x > max.x) {
		r.x = max.x;
	} else {
		r.x = p.x;
	}

	if (p.y < min.y) {
		r.y = min.y;
	} else if (p.y > max.y) {
		r.y = max.y;
	} else {
		r.y = p.y;
	}

	if (p.z < min.z) {
		r.z = min.z;
	} else if (p.z > max.z) {
		r.z = max.z;
	} else {
		r.z = p.z;
	}
	return r;  // Return it
}

//
// Based on 3D Math Primer for Games and Graphics Development book.
// which uses "Fast Ray-Box Intersection," by Woo in Graphics Gems I, page 395.
// check ray vs cube (could be a AABB).
//****Amended by SM.
bool AABB3::rayIntersect(Vector rayOrg, Vector rayEnd, Vector aabbPos){
	// Check for point inside box, trivial reject, and determine parametric distance to each front face
	float xt, yt, zt;
	Vector rayDelta = rayEnd - rayOrg;
	bool inside = true;

	Vector min, max;	//****
	min = this->min + aabbPos;
	max = this->max + aabbPos;

	if (rayOrg.x < min.x) {
		xt = min.x - rayOrg.x;
		if (xt > rayDelta.x) return false;
		xt /= rayDelta.x;
		inside = false;
	} else if (rayOrg.x > max.x) {
		xt = max.x - rayOrg.x;
		if (xt < rayDelta.x) return false;
		xt /= rayDelta.x;
		inside = false;
	} else
		xt = -1.0f;

	if (rayOrg.y < min.y) {
		yt = min.y - rayOrg.y;
		if (yt > rayDelta.y) return false;
		yt /= rayDelta.y;
		inside = false;
	} else if (rayOrg.y > max.y) {
		yt = max.y - rayOrg.y;
		if (yt < rayDelta.y) return false;
		yt /= rayDelta.y;
		inside = false;
	} else
		yt = -1.0f;

	if (rayOrg.z < min.z) {
		zt = min.z - rayOrg.z;
		if (zt > rayDelta.z) return false;
		zt /= rayDelta.z;
		inside = false;
	} else if (rayOrg.z > max.z) {
		zt = max.z - rayOrg.z;
		if (zt < rayDelta.z) return false;
		zt /= rayDelta.z;
		inside = false;
	} else
		zt = -1.0f;

	if (inside)
		return true;
	
	int which = 0;  // Select farthest plane - this is the plane of intersection.
	float t = xt;
	if (yt > t) {
		which = 1;
		t = yt;
	}
	if (zt > t) {
		which = 2;
		t = zt;
	}

	switch (which) {
		case 0:{ // intersect with yz plane
			float y = rayOrg.y + rayDelta.y*t;
			if (y < min.y || y > max.y) return false;
			float z = rayOrg.z + rayDelta.z*t;
			if (z < min.z || z > max.z) return false;
		} break;

		case 1:{ // intersect with xz plane
			float x = rayOrg.x + rayDelta.x*t;
			if (x < min.x || x > max.x) return false;
			float z = rayOrg.z + rayDelta.z*t;
			if (z < min.z || z > max.z) return false;
		} break;

		case 2:{ // intersect with xy plane
			float x = rayOrg.x + rayDelta.x*t;
			if (x < min.x || x > max.x) return false;
			float y = rayOrg.y + rayDelta.y*t;
			if (y < min.y || y > max.y) return false;
		} break;
	}  // t is parametric point of intersection
	return true;  
}

/////////////////////////////////////////////////////////////////////////////
//
// Global nonmember code
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
// intersectAABBs
//
// Check if two AABBs intersect, and return true if so.  Optionally return
// the AABB of their intersection, if an intersection is detected

//bool intersectAABBs(const AABB3 &box1, const AABB3 &box2, AABB3 *boxIntersect) {
bool AABB3::intersectAABBs(AABB3 box1, const Vector pos1, AABB3 box2, const Vector pos2, AABB3 *boxIntersect){	//****
	box1.min = box1.min + pos1;	//**** add positions of boxes before checking for intersection
	box2.min = box2.min + pos2;
	box1.max = box1.max + pos1;
	box2.max = box2.max + pos2;
	
	// Check for no overlap
	if (box1.min.x > box2.max.x) return false;
	if (box1.max.x < box2.min.x) return false;
	if (box1.min.y > box2.max.y) return false;
	if (box1.max.y < box2.min.y) return false;
	if (box1.min.z > box2.max.z) return false;
	if (box1.max.z < box2.min.z) return false;

	// We have overlap.  Compute AABB of intersection, if they want it
	if (boxIntersect != NULL) {
		boxIntersect->min.x = max(box1.min.x, box2.min.x);
		boxIntersect->max.x = min(box1.max.x, box2.max.x);
		boxIntersect->min.y = max(box1.min.y, box2.min.y);
		boxIntersect->max.y = min(box1.max.y, box2.max.y);
		boxIntersect->min.z = max(box1.min.z, box2.min.z);
		boxIntersect->max.z = min(box1.max.z, box2.max.z);
	}

	// They intersected
	return true;
}
