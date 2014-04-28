#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "vector.h"


class Marker
{
public:
	Marker(float x,float y,float z);
	~Marker(void);
	
	Vector verts[8];

	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3);
	void setPos(float x, float y, float z);
	void setRGB(float r, float g, float b);
	bool collides(Marker* mark);
	float sx, sy, sz;
	float px,py,pz;
	float mr, mg, mb;
	float Dx,Dy,Dz;
	Vector pos;
};

