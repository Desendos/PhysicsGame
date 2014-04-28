#include "Game.h"
#include "Box.h"


Game::Game(void){
	mouseX = mouseY = 0;
	camX = 0.0f;
	camY = 1.0f;
	camZ = camRad = 1.0f;
	fCount = 0;
	physicsState = false;
	placingWalls = false;
	wallNumber = 0;
}

Game::~Game(void){
}

void Game::InitOpenGL(){
	DebugOut("Game::InitOpenGL being called");
	Set2D();
}

void Game::Initialise(){
	DebugOut("Game::Initialise being called");
	glEnable(GL_POLYGON_SMOOTH);  //set best smoothing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	timer = new Timer();
	font1 = new BFont(hDC, "Courier", 14);
	initPhysicsObjects();
	oPlatform = new OGL_Box(pForm);
	oPlatform->setRGB(1.0f,1.0f,1.0f);
	/*oBox = new OGL_Box(pBox);
	oBox->setRGB(0.0f,0.0f,1.0f);*/
	for(int i = 0; i < ARRAY_BOX_NUMBER; i++){
		oBoxArray[i] = new OGL_Box(pBoxArray[i]);
		oBoxArray[i]->setRGB(1.0,0.0,0.0);
	}
	//oSphere = new OGL_Sphere(pSphere);
	//oSphere->setRGB(0.0,1.0,0.0);
	mark = new Marker(0.1,0.5,0.1);
	mark->setPos(0.0,0.8,3.0);
	mark->setRGB(0.0,1.0,0.0);

	goal = new Marker(0.2,0.2,0.2);
	goal->setPos(1.8,1.0,0.0);
	goal->setRGB(0.0,1.0,1.0);

}

void Game::Shutdown(){

	DebugOut("Game::Shutdown being called");
	delete timer;
	delete font1;
}

void Game::Update(){
	//Timers
	while(timer->getElapsedTime() - lft - 1.0f/60.0f < 0.001f); //Frame Limiter (set to 60 FPS)
	cft = timer->getElapsedTime();
	tbf = cft - lft;
	lft = cft;
	//Havok
	if(physicsState){
		m_world->stepDeltaTime(tbf); //update physics engine
		#ifdef _DEBUG
			m_vdb->step(tbf);	// update VDB when running
		#endif
	}

	//if(pSphere->getPos().y < -10.0){//to stop error
	//	physicsState = false;
	//}
		
	//Object Updates
	oPlatform->update();
	/*oSphere->update();*/
//	oBox->update();
	for(int i = 0; i < ARRAY_BOX_NUMBER; i++){
		oBoxArray[i]->update();
	}
	for(int i = 0; i < ARRAY_WALL_NUMBER; i++){
		if(oWall[i]){
			oWall[i]->update();
		}
	}
	mark->collides(goal);
	//Camera
	toX = pForm->getPos().x; toY = pForm->getPos().y; toZ = pForm->getPos().z;
}

void Game::RenderHUD(){
	Set2D(0, m_width, m_height, 0); // Change to 2D view and use 1:1 pixel resolution with [0,0] origin being at the top-left corner.
	glDisable(GL_DEPTH_TEST);  // Disable depth testing so the HUD will not be hidden by the 3D graphics
	glEnable(GL_BLEND);  // Semi-transparent background 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f, 0.0f, 0.9f, 0.5f);
	glBegin(GL_QUADS);
		glVertex2f(0,0);	//top of screen
		glVertex2f(0,20);
		glVertex2f(m_width, 20);
		glVertex2f(m_width, 0);
	glEnd();
	glDisable(GL_BLEND);
	
	font1->setColor(1.0f, 1.0f, 0.0f);
	strcpy_s(text, "Applied Physics - Platform");
	font1->printString(4, 15, text);
	sprintf(text, "Avg FPS: %.1f", (float)fCount / cft);
	font1->printString(350, 15, text);
	sprintf(text, "Walls: %i/%i", (int)wallNumber, (int)ARRAY_WALL_NUMBER);
	font1->printString(500, 15, text);

	Set3D(VIEW_ANGLE, NEAR_CLIPPING, FAR_CLIPPING); // Set back to 3D
}

// Render the objects in their current state.
void Game::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Camera
	if(placingWalls == false){
		gluLookAt(camX+10, camY+10, camZ, toX, toY, toZ, 0.0f, 1.0f, 0.0f);
	}

	if(placingWalls == true){
		gluLookAt(camX, camY+10, camZ, toX, toY, toZ, 0.0f, 1.0f, 0.0f);
	}
	
	//Objects Rendering
	oPlatform->render();
	mark->render();
	if(goal){
		goal->render();
	}
	//oSphere->render();
	//oBox->render();
	for(int i = 0; i < ARRAY_BOX_NUMBER; i++){
		oBoxArray[i]->render();
	}
	for(int i = 0; i < ARRAY_WALL_NUMBER; i++){
		if(oWall[i]){
			oWall[i]->render();
		}
	}
	//End
	RenderHUD();
	fCount++;
}

void Game::CameraPos(){
	if(placingWalls == false){
		angNS = ((mouseY + 1) / m_height) * (float)M_PI;  // Map the mouse position to two angles
		angEW = (mouseX / m_width) * 2 * (float)M_PI;
	
		float sinNS = sin(angNS);  // Calculate the sines and cosines of these angles
		float cosNS = cos(angNS);
		float sinEW = sin(angEW);
		float cosEW = cos(angEW);
	
		camZ = camRad * sinNS * cosEW;  // calculate the camera coordinate
		camY = camRad * cosNS;
		camX= camRad * sinNS * sinEW;
	}
}

void Game::initPhysicsObjects(){
	//IMPLEMENTATION OF WORLD
#ifdef _DEBUG
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initChecking( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(2*1024*1024) );
#else
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024*1024) );
#endif
	hkBaseSystem::init( memoryRouter, errorReport);
	hkpWorldCinfo worldInfo;	// Create the simulation world1
	worldInfo.m_gravity.set(0.0f,-9.8f,0.0f);
	worldInfo.setBroadPhaseWorldSize(100.0f);	//
	m_world = new hkpWorld(worldInfo); 
	m_world->lock();	//****
	// This is needed to detect collisionss
	hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
#ifdef _DEBUG
	m_physicsContext = new hkpPhysicsContext();//Connect VDB
	m_physicsContext->addWorld(m_world); // add all worlds
	hkpPhysicsContext::registerAllPhysicsProcesses();
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(m_physicsContext );  
	m_vdb = new hkVisualDebugger( contexts );
	m_vdb->serve();
#endif
	//OBJECTS
	if(!pForm){
		pForm = new Box(2.0,0.1,2.0);
		pForm->setPos(Vector(0.0,0.0,0.0));
		pForm->init(m_world);
	}
	if(!pBox){
	/*	pBox = new Box(0.2,0.2,0.2);
		pBox->setPos(Vector(0.0,1.2,0.0));
		pBox->init(m_world);*/
	}
	pSphere = new Sphere(0.2,0.2,0.2);
	pSphere->setPos(Vector(pForm->getPos().x,3.0,pForm->getPos().z));
	pSphere->init(m_world);
	
	pBoxArray[0] = new Box(0.2,0.2,0.2);
	pBoxArray[0]->setPos(Vector(0.0, 0.4, 0.4));
	pBoxArray[0]->init(m_world);

	pBoxArray[1] = new Box(0.2,0.2,0.2);
	pBoxArray[1]->setPos(Vector(0.0, 0.4, -0.4));
	pBoxArray[1]->init(m_world);
	
	pBoxArray[2] = new Box(0.2,0.2,0.2);
	pBoxArray[2]->setPos(Vector(0.4, 0.4, 0.0));
	pBoxArray[2]->init(m_world);

	pBoxArray[3] = new Box(0.2,0.2,0.2);
	pBoxArray[3]->setPos(Vector(-0.4, 0.4, 0.0));
	pBoxArray[3]->init(m_world);

	//makeWall();

	//Const 1
	hkpGenericConstraintData* data = new hkpGenericConstraintData();	// Create constraint on the level
	hkpConstraintConstructionKit kit;
	kit.begin(data);
		kit.setPivotA(hkVector4(0.0f, 0.0f, 0.0f));	
		kit.setPivotB(hkVector4(0.0f, 0.0f, 0.0f));
		kit.constrainAllLinearDof();
		kit.setAngularBasisABodyFrame();
		kit.setAngularBasisBBodyFrame();

		kit.setAngularLimit(ANGLE_LIMIT, -ANGLE_LIMIT, ANGLE_LIMIT);	
		//// do not limit rotation around Y axis
		kit.setAngularLimit(2, 0.0f, 2);  
	kit.end();
	hkpConstraintInstance* constraint = new hkpConstraintInstance(pForm->getRigidBody(), NULL, data);
	m_world->addConstraint(constraint);
	data->removeReference();
	constraint->removeReference();

	//END
	m_world->unlock();
}

void Game::makeWall(){
	if(wallNumber < ARRAY_WALL_NUMBER){
		pWall[wallNumber] = new Box(0.1,0.5,0.1);
		pWall[wallNumber]->setPos(Vector(mark->px,mark->py,mark->pz));
		pWall[wallNumber]->initFixed(m_world);
		oWall[wallNumber] = new OGL_Box(pWall[wallNumber]);
		oWall[wallNumber]->setRGB(1.0, 0.6, 0.0);
		wallNumber++;
	}
}

void Game::moveWeight1Up(){
	pBoxArray[0]->getRigidBody()->setPosition(hkVector4(pBoxArray[0]->getPos().x,pBoxArray[0]->getPos().y,pBoxArray[0]->getPos().z+0.1));

	pBoxArray[1]->getRigidBody()->setPosition(hkVector4(pBoxArray[1]->getPos().x,pBoxArray[1]->getPos().y,pBoxArray[1]->getPos().z+0.1));
}

void Game::moveWeight1Down(){
	pBoxArray[0]->getRigidBody()->setPosition(hkVector4(pBoxArray[0]->getPos().x,pBoxArray[0]->getPos().y,pBoxArray[0]->getPos().z-0.1));

	pBoxArray[1]->getRigidBody()->setPosition(hkVector4(pBoxArray[1]->getPos().x,pBoxArray[1]->getPos().y,pBoxArray[1]->getPos().z-0.1));
}

void Game::moveWeight2Up(){
	pBoxArray[2]->getRigidBody()->setPosition(hkVector4(pBoxArray[2]->getPos().x+0.1,pBoxArray[2]->getPos().y,pBoxArray[2]->getPos().z));

	pBoxArray[3]->getRigidBody()->setPosition(hkVector4(pBoxArray[3]->getPos().x+0.1,pBoxArray[3]->getPos().y,pBoxArray[3]->getPos().z));
}

void Game::moveWeight2Down(){
	pBoxArray[2]->getRigidBody()->setPosition(hkVector4(pBoxArray[2]->getPos().x-0.1,pBoxArray[2]->getPos().y,pBoxArray[2]->getPos().z));

	pBoxArray[3]->getRigidBody()->setPosition(hkVector4(pBoxArray[3]->getPos().x-0.1,pBoxArray[3]->getPos().y,pBoxArray[3]->getPos().z));
}

void Game::makeWeightsJump(){
	pBoxArray[0]->getRigidBody()->setPosition(hkVector4(pBoxArray[0]->getPos().x,pBoxArray[0]->getPos().y+0.1,pBoxArray[0]->getPos().z));
	pBoxArray[1]->getRigidBody()->setPosition(hkVector4(pBoxArray[1]->getPos().x,pBoxArray[1]->getPos().y+0.1,pBoxArray[1]->getPos().z));
	pBoxArray[2]->getRigidBody()->setPosition(hkVector4(pBoxArray[2]->getPos().x,pBoxArray[2]->getPos().y+0.1,pBoxArray[2]->getPos().z));
	pBoxArray[3]->getRigidBody()->setPosition(hkVector4(pBoxArray[3]->getPos().x,pBoxArray[3]->getPos().y+0.1,pBoxArray[3]->getPos().z));
}

void Game::moveMarkerUp(){
	mark->setPos(mark->px+0.1, mark->py, mark->pz);
}
void Game::moveMarkerDown(){
	mark->setPos(mark->px-0.1, mark->py, mark->pz);
}
void Game::moveMarkerLeft(){
	mark->setPos(mark->px, mark->py, mark->pz+0.1);
}
void Game::moveMarkerRight(){
	mark->setPos(mark->px, mark->py, mark->pz-0.1);
}