/*
	Code Based on
	OpenGL Game Programming Ch. 18
	by Kevin Hawkins and Dave Astle
*/
#include "MD2Model.h"

void FreeModel(modelData_t *model){
	if (model->triIndex != NULL)
		free(model->triIndex);
	if (model->pointList != NULL)
		free(model->pointList);
	if (model->st != NULL)
		free(model->st);
	if (model != NULL)
		free(model);
}

//pl, p2 & p3 should be in CW order.  Returns unit normal to plane
void NormToPlane(Vector p1, Vector p2, Vector p3){	
	Vector v1, v2, n;
	v1 = p1 - p2;
	v2 = p3 - p2;
	n = v1.CrossProduct(v2);
	if(n.Length() > 0) n.Normalize();
	glNormal3f(n.x, n.y, n.z);  //set OpenGL normals
}

MD2Model::MD2Model(){
	interValue = 0;
	myModel = gunModel = NULL;
	alpha = 0.4f;
}

MD2Model::~MD2Model(){
	FreeModel(myModel);
	FreeModel(gunModel);
}

texture_t* MD2Model::LoadBMPTexture(char *filename){
	texture_t *thisTexture;			// the texture
	LoadTextures *lt = new LoadTextures(filename);
	
	thisTexture = new texture_t();
	thisTexture->texID = lt->getNum();
	thisTexture->height = lt->textureImage.bmHeight;
	thisTexture->width = lt->textureImage.bmWidth;
	thisTexture->palette = NULL;
	thisTexture->scaledHeight = 0;
	thisTexture->scaledWidth = 0;
	thisTexture->textureType = BMP;	
	
	delete lt;
	return thisTexture;
}


// desc: loads an MD2 model specified by filename and with the texture
// specified by textureName
modelData_t* MD2Model::LoadMD2Model(char *filename, char *textureName){
	FILE *filePtr;						// file pointer
	int fileLen;						// length of model file
    char *buffer;						// file buffer
		
	modelData_t *model;					// the model
	modelHeader_t *modelHeader;			// model header
	texture_t *md2Texture;				// model texture

	stIndex_t *stPtr;					// texture data
    frame_t *frame;						// frame data
	vector_t *pointListPtr;				// index variable
    mesh_t *triIndex, *bufIndexPtr;		// index variables
    int i, j;							// index variables

	filePtr = fopen(filename, "rb"); // open the model file
	if (filePtr == NULL)
		return NULL;

	// find length of file
    fseek(filePtr, 0, SEEK_END);
    fileLen = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
	
    // read entire file into buffer
    buffer = (char*)malloc(fileLen + 1);
    fread(buffer, sizeof(char), fileLen, filePtr);

	// extract model file header from buffer
    modelHeader = (modelHeader_t*)buffer;

	// allocate memory for model data
   	model = (modelData_t*)malloc(sizeof(modelData_t));
	if (model == NULL)
		return NULL;

	// allocate memory for all vertices used in model, including animations
    model->pointList = (vector_t*)malloc(sizeof(vector_t)*modelHeader->numXYZ * modelHeader->numFrames);

	// store vital model data
    model->numPoints = modelHeader->numXYZ;
    model->numFrames = modelHeader->numFrames;
	model->frameSize = modelHeader->framesize;

    // loop number of frames in model file
    for(j = 0; j < modelHeader->numFrames; j++){
       // offset to the points in this frame
       frame = (frame_t*)&buffer[modelHeader->offsetFrames + modelHeader->framesize * j];

	   // calculate the point positions based on frame details
       pointListPtr = (vector_t*)&model->pointList[modelHeader->numXYZ * j];
       for(i = 0; i < modelHeader->numXYZ; i++){
		  pointListPtr[i].point.x = frame->scale.x * frame->fp[i].v[0] + frame->translate.x;
		  pointListPtr[i].point.y = frame->scale.y * frame->fp[i].v[1] + frame->translate.y;
		  pointListPtr[i].point.z = frame->scale.z * frame->fp[i].v[2] + frame->translate.z;
       }
    }
			 
	// load the model texture
	md2Texture = LoadBMPTexture(textureName);
	if (md2Texture != NULL)	{
		// setup texture for opengl and store it in model data structure
		glBindTexture(GL_TEXTURE_2D, md2Texture->texID);
		model->modelTex = md2Texture;
	}
	else
		return NULL;

    // allocate memory for the model texture coordinates
    model->st = (texCoord_t*)malloc(sizeof(texCoord_t)*modelHeader->numST);

	// store number of texture coordinates
    model->numST = modelHeader->numST;

	// set texture pointer to texture coordinate offset
    stPtr = (stIndex_t*)&buffer[modelHeader->offsetST];

	// calculate and store the texture coordinates for the model
    for (i = 0; i < modelHeader->numST; i++) {
		model->st[i].s = (float)stPtr[i].s / (float)md2Texture->width;
        model->st[i].t = (float)stPtr[i].t / (float)md2Texture->height;
    }

	// allocate an index of triangles
	triIndex = (mesh_t*)malloc(sizeof(mesh_t) * modelHeader->numTris);

	// set total number of triangles
	model->numTriangles = modelHeader->numTris;
	model->triIndex = triIndex;
	
	// point to triangle indexes in buffer
	bufIndexPtr = (mesh_t*)&buffer[modelHeader->offsetTris];

	// create a mesh (triangle) list
	for (j = 0; j < model->numFrames; j++){
		for(i = 0; i < modelHeader->numTris; i++){// for all triangles in each frame
		   for(int k = 0; k < 3; k++){
			   triIndex[i].meshIndex[k] = bufIndexPtr[i].meshIndex[k];
			   triIndex[i].stIndex[k] = bufIndexPtr[i].stIndex[k];
			}
		}
	}

	// close file and free memory
	fclose(filePtr);
    free(buffer);

	model->currentFrame = 0;
	model->nextFrame = 1;
	model->interpol = 0.0;

	myModel = model;  //****
	return model;
}

// DisplayMD2()
// desc: displays the frameNum frame of an MD2 model
void MD2Model::DisplayMD2(int frameNum){
	modelData_t *model;
	if(myModel != NULL)  //****use myModel, if available
		model = myModel;
	else
		return;
	
	vector_t *pointList;

    // create a pointer to the frame we want to show
    pointList = &model->pointList[model->numPoints * frameNum];
	glBindTexture(GL_TEXTURE_2D, model->modelTex->texID);  //set the texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);	// display blending
    
	glBegin(GL_TRIANGLES);
		for(int i = 0; i < model->numTriangles; i++){
			NormToPlane(pointList[model->triIndex[i].meshIndex[0]].point,
                             pointList[model->triIndex[i].meshIndex[1]].point,
                             pointList[model->triIndex[i].meshIndex[2]].point);
			glTexCoord2f(model->st[model->triIndex[i].stIndex[0]].s,
						 model->st[model->triIndex[i].stIndex[0]].t);
			glVertex3f(pointList[model->triIndex[i].meshIndex[0]].point.x, pointList[model->triIndex[i].meshIndex[0]].point.y,
				pointList[model->triIndex[i].meshIndex[0]].point.z);

			glTexCoord2f(model->st[model->triIndex[i].stIndex[2]].s ,
					     model->st[model->triIndex[i].stIndex[2]].t);
			glVertex3f(pointList[model->triIndex[i].meshIndex[2]].point.x, pointList[model->triIndex[i].meshIndex[2]].point.y,
				pointList[model->triIndex[i].meshIndex[2]].point.z);

			glTexCoord2f(model->st[model->triIndex[i].stIndex[1]].s,
					     model->st[model->triIndex[i].stIndex[1]].t);
			glVertex3f(pointList[model->triIndex[i].meshIndex[1]].point.x, pointList[model->triIndex[i].meshIndex[1]].point.y,
				pointList[model->triIndex[i].meshIndex[1]].point.z);
		}
    glEnd();
	glDisable(GL_BLEND);
}

void MD2Model::getTriPoints(modelData_t *model, int ti){
	vector_t *pointList = &model->pointList[model->numPoints*model->currentFrame];
	vector_t *nextPointList = &model->pointList[model->numPoints*model->nextFrame];
	vector_t vertex[3];
	float x1, y1, z1;				// current frame point values
	float x2, y2, z2;				// next frame point values

	// get first points of each frame
	x1 = pointList[model->triIndex[ti].meshIndex[0]].point.x;
	y1 = pointList[model->triIndex[ti].meshIndex[0]].point.y;
	z1 = pointList[model->triIndex[ti].meshIndex[0]].point.z;
	x2 = nextPointList[model->triIndex[ti].meshIndex[0]].point.x;
	y2 = nextPointList[model->triIndex[ti].meshIndex[0]].point.y;
	z2 = nextPointList[model->triIndex[ti].meshIndex[0]].point.z;

	// xi + percentage * (xf - xi)
	// store first interpolated vertex of triangle
	vertex[0].point.x = x1 + model->interpol * (x2 - x1);
	vertex[0].point.y = y1 + model->interpol * (y2 - y1);
	vertex[0].point.z = z1 + model->interpol * (z2 - z1);
		
	// get second points of each frame
	x1 = pointList[model->triIndex[ti].meshIndex[2]].point.x;
	y1 = pointList[model->triIndex[ti].meshIndex[2]].point.y;
	z1 = pointList[model->triIndex[ti].meshIndex[2]].point.z;
	x2 = nextPointList[model->triIndex[ti].meshIndex[2]].point.x;
	y2 = nextPointList[model->triIndex[ti].meshIndex[2]].point.y;
	z2 = nextPointList[model->triIndex[ti].meshIndex[2]].point.z;

	// store second interpolated vertex of triangle
	vertex[2].point.x = x1 + model->interpol * (x2 - x1);
	vertex[2].point.y = y1 + model->interpol * (y2 - y1);
	vertex[2].point.z = z1 + model->interpol * (z2 - z1);	
	
	// get third points of each frame
	x1 = pointList[model->triIndex[ti].meshIndex[1]].point.x;
	y1 = pointList[model->triIndex[ti].meshIndex[1]].point.y;
	z1 = pointList[model->triIndex[ti].meshIndex[1]].point.z;
	x2 = nextPointList[model->triIndex[ti].meshIndex[1]].point.x;
	y2 = nextPointList[model->triIndex[ti].meshIndex[1]].point.y;
	z2 = nextPointList[model->triIndex[ti].meshIndex[1]].point.z;

	// store third interpolated vertex of triangle
	vertex[1].point.x = x1 + model->interpol * (x2 - x1);
	vertex[1].point.y = y1 + model->interpol * (y2 - y1);
	vertex[1].point.z = z1 + model->interpol * (z2 - z1);

	// calculate the normal of the triangle
	NormToPlane(vertex[0].point, vertex[1].point, vertex[2].point); //use CW ordering

	// render properly textured triangle
	glTexCoord2f(model->st[model->triIndex[ti].stIndex[0]].s,
					model->st[model->triIndex[ti].stIndex[0]].t);
	glVertex3f(vertex[0].point.x, vertex[0].point.y, vertex[0].point.z);

	glTexCoord2f(model->st[model->triIndex[ti].stIndex[2]].s ,
					model->st[model->triIndex[ti].stIndex[2]].t);
	glVertex3f(vertex[2].point.x, vertex[2].point.y, vertex[2].point.z);

	glTexCoord2f(model->st[model->triIndex[ti].stIndex[1]].s,
					model->st[model->triIndex[ti].stIndex[1]].t);
	glVertex3f(vertex[1].point.x, vertex[1].point.y, vertex[1].point.z);
}

// DisplayMD2Interpolate()
// desc: displays an animated MD2 model with keyframe interpolation
void MD2Model::DisplayMD2Interpolate(){
	modelData_t *model;
	if(myModel != NULL)  //****use myModel, if available
		model = myModel;
	else
		return;

	if (model->interpol >= 1.0)	{
		model->interpol = 0.0f;
		model->currentFrame++;
		if (model->currentFrame >= model->numFrames)
			model->currentFrame = 0;

		model->nextFrame = model->currentFrame + 1;

		if (model->nextFrame >= model->numFrames)
			model->nextFrame = 0;
	}

	glBindTexture(GL_TEXTURE_2D, model->modelTex->texID);
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < model->numTriangles; i++)
			getTriPoints(model, i);
	glEnd();
	model->interpol += 0.05f;	// increase percentage of interpolation between frames
}

// DisplayMD2Interpolate()
// desc: displays a frame of the model between startFrame and endFrame with an interpolation percent
void MD2Model::DisplayMD2Interpolate(int startFrame, int endFrame, float percent){
	modelData_t *model;
	if(myModel != NULL)  //****use myModel, if available
		model = myModel;

	if(model == NULL)
		return;
	
	if((startFrame < 0) || (endFrame < 0))
		return;

	if((startFrame >= model->numFrames) || (endFrame >= model->numFrames))
		return;

	if(model->interpol == 0.0)
		model->currentFrame = startFrame;

	if(model->interpol >= 1.0){
		model->interpol = 0.0f;
		model->currentFrame++;
		if (model->currentFrame >= endFrame)//model->numFrames)
			model->currentFrame = startFrame; //0;

		model->nextFrame = model->currentFrame + 1;
		if (model->nextFrame >= endFrame)
			model->nextFrame = startFrame;
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);	// display blending

	glBindTexture(GL_TEXTURE_2D, model->modelTex->texID);
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < model->numTriangles; i++)
			getTriPoints(model, i);
	glEnd();
	glDisable(GL_BLEND);
	model->interpol += percent;	// increase percentage of interpolation between frames
}
