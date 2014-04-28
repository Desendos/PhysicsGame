#pragma once
#include "hvkoglobj.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Box.h"

#include "vector.h"
class OGL_Level :
	public HvkOGLObj
{
public:
	OGL_Level(Box* hvkBox);
	~OGL_Level(void);

	Vector verts[8];
	
	void render();
	void setSize(float sx, float sy, float sz);
	void drawFace(int v0, int v1, int v2, int v3, float r, float g, float b);
	int texture[6];
	void reloadTextures();
	unsigned int texNum;
	Vector fNorm;

};

