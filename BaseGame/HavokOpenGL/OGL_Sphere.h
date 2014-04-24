#pragma once
#include "hvkoglobj.h"
#include "Sphere.h"
class OGL_Sphere :
	public HvkOGLObj
{
public:
	OGL_Sphere(Sphere* hvkBox);
	~OGL_Sphere(void);

	Vector verts[8];
	
	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3);
};

