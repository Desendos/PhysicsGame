#include "OGL_Level.h"


OGL_Level::OGL_Level(Level* hvkBox) : HvkOGLObj(hvkBox){
	setSize(hvkBox->sx, hvkBox->sy, hvkBox->sz);
	setRGB();
}

void OGL_Level::render(){
	float matSpec[] = {1.0f, 0.0f, 0.0f, 1.0f };
	float matShiny[] = {100.0 };  //128 is max value
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, matSpec);
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShiny);
		glColor3f(r, g, b);
		// Perform transformations here in TRS order
		glTranslatef(hObj->getPos().x, hObj->getPos().y, hObj->getPos().z);
		glRotatef(angle,hObj->getDir().x,hObj->getDir().y,hObj->getDir().z);
		drawFace(0, 4, 5, 1, 1.0f, 1.0f, 1.0f); // Draw each face
		drawFace(3, 7, 4, 0, 1.0f, 1.0f, 1.0f);
		drawFace(2, 6, 7, 3, 1.0f, 1.0f, 1.0f);
		drawFace(1, 5, 6, 2, 1.0f, 1.0f, 1.0f);
		drawFace(3, 0, 1, 2, 1.0f, 1.0f, 1.0f);
		drawFace(4, 7, 6, 5, 1.0f, 1.0f, 1.0f);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

OGL_Level::~OGL_Level(void){
}

void OGL_Level::drawFace(int v0, int v1, int v2, int v3, float r, float g, float b){
//	glColor3f(r, g, b);
	glBindTexture(GL_TEXTURE_2D, texNum);
	glNormal3f(fNorm.x, fNorm.y, fNorm.z);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(verts[v0].x, verts[v0].y, verts[v0].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(verts[v1].x, verts[v1].y, verts[v1].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(verts[v2].x, verts[v2].y, verts[v2].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(verts[v3].x, verts[v3].y, verts[v3].z);
	glEnd();
}

void OGL_Level::setSize(float sx,float sy, float sz){
	verts[0].set(-sx, sy, sz);
	verts[1].set( sx, sy, sz);
	verts[2].set( sx, sy,-sz);
	verts[3].set(-sx, sy,-sz);
	verts[4].set(-sx,-sy, sz);
	verts[5].set( sx,-sy, sz);
	verts[6].set( sx,-sy,-sz);
	verts[7].set(-sx,-sy,-sz);
}

void OGL_Level::reloadTextures(){
	//for(int i = 0; i < 6; i++){
	//LoadTextures *lt = new LoadTextures("Images/rock.bmp");
	//if (!lt) return; 
	//texNum = lt->getTexture();
	//delete lt;
}
