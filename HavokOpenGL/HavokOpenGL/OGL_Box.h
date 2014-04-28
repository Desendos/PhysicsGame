#pragma once
#include "hvkoglobj.h"
#include "Box.h"
class OGL_Box :
	public HvkOGLObj
{
public:
	OGL_Box(Box* hvkBox);
	~OGL_Box(void);

	Vector verts[8];
	
	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3);
};

