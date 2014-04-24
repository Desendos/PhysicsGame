/*
	Code Based on OpenGL Game Programming Ch. 18
	by Kevin Hawkins and Dave Astle
*/
#ifndef MD2MODEL_H
#define MD2MODEL_H

#include "md2.h"
#include "LoadTextures.h"
#include "Vector.h"
#include "AABB3.h"

class MD2Model{
	modelData_t *myModel;
	modelData_t *gunModel;
	float interValue;
public:
	AABB3 bb;  //****added by SM for easy access
	float alpha;
	Vector pos;

	MD2Model();
	~MD2Model();
	modelData_t* LoadMD2Model(char *filename, char *textureName);
	void DisplayMD2(int frameNum);
	void DisplayMD2Interpolate();
	void DisplayMD2Interpolate(int startFrame, int endFrame, float percent);
private:
	texture_t* LoadBMPTexture(char *filename);
	void getTriPoints(modelData_t *model, int tIdx);
};

#endif