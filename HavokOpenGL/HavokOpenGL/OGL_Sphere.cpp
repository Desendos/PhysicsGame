#include "OGL_Sphere.h"



OGL_Sphere::OGL_Sphere(Sphere* hvkBox) : HvkOGLObj(hvkBox){
	setSize(hvkBox->getRadius());
	
	setRGB();
	colSphere = gluNewQuadric();
	gluQuadricDrawStyle(colSphere, GLU_LINE);
	gluQuadricNormals(colSphere, GLU_NONE);
}

void OGL_Sphere::render(){
	glPushMatrix();
		glColor3f(r, g, b);
		// Perform transformations here in TRS order
		glTranslatef(hObj->getPos().x, hObj->getPos().y, hObj->getPos().z);
		glRotatef(angle,hObj->getDir().x,hObj->getDir().y,hObj->getDir().z);
		//drawFace(0, 4, 5, 1); // Draw each face
		//drawFace(3, 7, 4, 0);
		//drawFace(2, 6, 7, 3);
		//drawFace(1, 5, 6, 2);
		//drawFace(3, 0, 1, 2);
		//drawFace(4, 7, 6, 5);
		glColor3f(1.0f, 0.0f, 0.5f);
		gluSphere(colSphere, 0.5, 10, 6);
	glPopMatrix();
}

OGL_Sphere::~OGL_Sphere(void){
	if(colSphere != NULL)
		gluDeleteQuadric(colSphere);
}

void OGL_Sphere::drawFace(int v0, int v1, int v2, int v3){
	glBegin(GL_QUADS);
		glVertex3f(verts[v0].x, verts[v0].y, verts[v0].z);
		glVertex3f(verts[v1].x, verts[v1].y, verts[v1].z);
		glVertex3f(verts[v2].x, verts[v2].y, verts[v2].z);
		glVertex3f(verts[v3].x, verts[v3].y, verts[v3].z);
	glEnd();
}

void OGL_Sphere::setSize(float size){
	float s = (float) size;
	float hs = size * size;

	csRadius = hs * sqrt(3.0f);
}

