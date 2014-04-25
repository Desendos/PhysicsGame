#pragma once
#include "hvkoglobj.h"
#include "Level.h"
class OGL_Level :
	public HvkOGLObj
{
public:
	OGL_Level(Level* hvkBox);
	~OGL_Level(void);

	Vector verts[8];
	
	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3);
};

