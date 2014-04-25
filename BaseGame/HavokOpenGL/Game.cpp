#include "Game.h"

Vector NormToPlane(Vector p1, Vector p2, Vector p3){	
	Vector v1, v2, n;
	v1 = p1 - p2;
	v2 = p3 - p2;
	n = v1.CrossProduct(v2);
	if(n.Length() > 0) n.Normalize();
	return n;
}

Game::Game(void){
	mouseX = mouseY = 0;
	camX = 0.0f;
	camY = 1.0f;
	camZ = camRad = 1.0f;
	fCount = 0;
	intCalled = false;
}

Game::~Game(void){
}

void Game::InitOpenGL(){
	DebugOut("Game::InitOpenGL being called");
	Set2D();
	
}

void Game::Initialise(){
	guiState = MAINSTATE;
	DebugOut("Game::Initialise being called");
	glEnable(GL_POLYGON_SMOOTH);  //set best smoothing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	if(timer == NULL){
		timer = new Timer();
	}
	if(font1 == NULL){
		font1 = new BFont(hDC, "Courier", 14);
	}


	gameState =  false;
	droppingY = 0.0f;
	//oSphere = new OGL_Sphere(pSphere);
	//oSphere->setRGB(1.0f, 0.0f, 0.0f);	
	lFact = new LevelFactoryImplementation();
}

void Game::Shutdown(){
	DebugOut("Game::Shutdown being called");
	delete timer;
	delete font1;
	//delete pForm;
	//delete pbox;
	//delete pSphere;
	//delete oPlatform;
	//delete oBox;
	//delete oSphere;
}

void Game::Update(){
	cft = timer->getElapsedTime();
	tbf = cft - lft;
	lft = cft;
	if(guiState == MAINSTATE){
		
	}

	if(guiState == GAMESTATE){
		if(gameState){
			m_world->stepDeltaTime(tbf); //update physics engine
			#ifdef _DEBUG
				m_vdb->step(tbf);	// update VDB when running
			#endif
		}
		/*if(oPlatform){
			oPlatform->update();
		}*/
		if(oLevel1){
			oLevel1->update();
		}
		if(oLevel2){
			oLevel2->update();
		}
		if(oBox){
			oBox->update();
		}
		toX = level1->getPos().x; toY = level1->getPos().y; toZ = level1->getPos().z;

		//oSphere->update();
		//if(oSphere->getHavokObj()->getPos().y < -15.0)//to stop error
		//	gameState = false;
	}
	



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
	strcpy_s(text, "Ball Dropper");
	font1->printString(4, 15, text);
	sprintf(text, "Avg FPS: %.1f", (float)fCount / cft);
	font1->printString(400, 15, text);

	Set3D(VIEW_ANGLE, NEAR_CLIPPING, FAR_CLIPPING); // Set back to 3D
}

void Game::renderMainMenu(){
	Set2D(0, m_width, m_height, 0); // Change to 2D view and use 1:1 pixel resolution with [0,0] origin being at the top-left corner.
	glDisable(GL_DEPTH_TEST);  // Disable depth testing so the HUD will not be hidden by the 3D graphics
	glEnable(GL_BLEND);  // Semi-transparent background 
	
	font1->setColor(1.0f, 1.0f, 0.0f);
	sprintf(text, "Main Menu");
	font1->printString(m_width/2, m_height/2, text);

	Set3D(VIEW_ANGLE, NEAR_CLIPPING, FAR_CLIPPING); // Set back to 3D
}
void Game::renderEndMenu(){
	Set2D(0, m_width, m_height, 0); // Change to 2D view and use 1:1 pixel resolution with [0,0] origin being at the top-left corner.
	glDisable(GL_DEPTH_TEST);  // Disable depth testing so the HUD will not be hidden by the 3D graphics
	glEnable(GL_BLEND);  // Semi-transparent background 
	
	font1->setColor(1.0f, 1.0f, 0.0f);
	sprintf(text, "Game Over");
	font1->printString(m_width/2, m_height/2, text);

	Set3D(VIEW_ANGLE, NEAR_CLIPPING, FAR_CLIPPING); // Set back to 3D
}

// Render the objects in their current state.
void Game::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(camX+200, camY+200, camZ, toX, toY, toZ, 0.0f, 1.0f, 0.0f);
	glEnable(GL_TEXTURE_2D); 
	if(guiState == GAMESTATE){
//		oPlatform->render();
		oBox->render();
		if(oLevel1){ 
			oLevel1->render();
		}
		if(oLevel2){
			oLevel2->render();
		}
		if(md2m){
			renderPlayer();
		}
		
		/*glLightfv(GL_LIGHT0, GL_POSITION, lightPos);*/

	}
	if(guiState == MAINSTATE){
		renderMainMenu();	
	}
	if(guiState == ENDSTATE){
		renderEndMenu();	
	}
	glDisable(GL_TEXTURE_2D);

	RenderHUD();
	fCount++;
}

void Game::CameraPos(){
	// Map the mouse position to two angles
	angNS = ((mouseY + 1) / m_height) * (float)M_PI;
	angEW = (mouseX / m_width) * 2 * (float)M_PI;
	// Calculate the sines and cosines of these angle
	float sinNS = sin(angNS);
	float cosNS = cos(angNS);
	float sinEW = sin(angEW);
	float cosEW = cos(angEW);
	// calculate the camera coordinate
	camZ = toZ + camRad * sinNS * cosEW;
	camY = toY + camRad * cosNS;
	camX = toX + camRad * sinNS * sinEW;
}

void Game::initPhysicsObjects(){
	if(intCalled == false){
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
	}

	m_world->lock();	//****

	/*if(pForm == NULL){
		pForm = new box(2.0f,0.1f,2.0f);
	}
	pForm->setPos(Vector(1.0f, droppingY, 0.0f));
	pForm->init(m_world);*/
	createLevel1();
	//createLevel2();
	if(pbox == NULL){
		pbox = new box(0.5f,0.5f,0.5f);
	}
	pbox->setPos(Vector(0.0f, 1.5f, 0.0f));
	pbox->init(m_world);
	/*createLevel1();
	createLevel2();*/

	/*if(pSphere == NULL){
		pSphere =  new Sphere(2.0f,0.1f,2.0f);
	}
	pForm->setPos(Vector(1.2f, 1.2f, 0.0f));
	pSphere->init(m_world);*/

	hkpGenericConstraintData* data = new hkpGenericConstraintData();	// Create constraint on the level
	hkpConstraintConstructionKit kit;
	kit.begin(data);
		kit.setPivotA(hkVector4(0.0f, 0.0f, 0.0f));	
		kit.setPivotB(hkVector4(1.0f, 0.0f, 0.0f));
		kit.constrainAllLinearDof();
		kit.setAngularBasisABodyFrame();
		kit.setAngularBasisBBodyFrame();

		kit.setAngularLimit(0, -ANGLE_LIMIT, ANGLE_LIMIT);	
		//// do not limit rotation around Y axis
		kit.setAngularLimit(2, 0.0f, 0.0f);  //stop z-axis rotation
	kit.end();
//	hkpConstraintInstance* constraint = new hkpConstraintInstance(pForm->getRigidBody(), NULL, data);
//	m_world->addConstraint(constraint);

	data->removeReference();
//	constraint->removeReference();

	m_world->unlock();
}

void Game::removeGameObjects(){
	guiState = ENDSTATE;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//pForm->removeRigidBody(m_world);
//	delete pForm;
	//pForm = NULL;
	pbox->removeRigidBody(m_world);
	delete pbox;
	pbox = NULL;
	//delete pSphere;
	//delete oPlatform;
	//oPlatform = NULL;
	delete oBox;
	oBox = NULL;
	delete oLevel1;
	oLevel1 = NULL;
	delete oLevel2;
	oLevel2 = NULL;
	destroyLevel1();
	//destroyLevel2();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	
}

void Game::createGameObjects(){
	gameState =  false;
	initPhysicsObjects();
	
	intCalled = true;
	//}
	/*if(oSphere == NULL){
		oSphere = new OGL_Sphere(pSphere);
	}
	oSphere->setRGB(1.0f, 0.0f, 0.0f);*/
	if(oPlatform == NULL){
//		oPlatform = new OGL_Box(pForm);
	}
	if(oBox == NULL){
		oBox = new OGL_Box(pbox);
	}
	
	
	//oPlatform->setRGB(1.0f, 0.5f, 0.5f);
	oBox->setRGB(0.0f, 0.0f, 1.0f);

	md2m = new MD2Model;
	md2m->LoadMD2Model("Data/pknight/pknight.md2", "Data/pknight/pknight.bmp");
	md2m->pos = Vector(0.0f, 0.0f, 0.0f);	
	

	//
	//float matSpec[] = {0.0f, 1.0f, 0.0f, 1.0f };
	//float matShiny[] = {50.0 };  //128 is max value
	//glMaterialfv(GL_FRONT, GL_AMBIENT, matSpec);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	//glMaterialfv(GL_FRONT, GL_SHININESS, matShiny);
	//lightPos[0]=0; lightPos[1]= 0; lightPos[2]= 0; lightPos[3]=1.0f;
	//float whiteLight[] = {2.0f, 2.0f, 2.0f, 1.0f };
	//float ambLight[] = {1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);


	

	guiState = GAMESTATE;
}

void Game::createLevel1(){
	if(level1 == NULL){
		level1 = lFact->createLevel(Normal, 20.0f,0.1f,20.0f);
	}
	droppingY = droppingY - 1.0f;
	level1->setPos(Vector(0.0f, droppingY, 0.0f));
	level1->init(m_world);
	if(oLevel1 == NULL){
		oLevel1 = new OGL_Level(level1);
	}
	oLevel1->setRGB(1.0f, 1.0f, 1.0f);
	oLevel1->reloadTextures();
}

void Game::createLevel2(){
	if(level2 == NULL){
	level2 = lFact->createLevel(Normal, 2.0f,0.1f,2.0f);
	}
	droppingY = droppingY - 1.0f;
	level2->setPos(Vector(20000.0f, 20.0f, 20.0f));
	level2->init(m_world);
	if(oLevel2 == NULL){
		oLevel2 = new OGL_Level(level2);
	}
	oLevel2->setRGB(1.0f, 1.0f, 1.0f);
}

void Game::destroyLevel1(){
	level1->removeRigidBody(m_world);
	delete level1;
	level1 = NULL;
	delete oLevel1;
	oLevel1 = NULL;
}

void Game::destroyLevel2(){
	level2->removeRigidBody(m_world);
	delete level2;
	level2 = NULL;
	delete oLevel2;
	oLevel2 = NULL;
}

void Game::dropBall(){
	pbox->setPos(Vector(0.0f, 3.0f, 0.0f));
	level1->setDir(level1->getDir() + Vector(0.0f,1.0f,0.0f));
}

void Game::moveRotation(Level* lev){
	lev->setRot();
}

void Game::renderPlayer(){
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(md2m->pos.x, md2m->pos.y, md2m->pos.z);	
		glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
		md2m->DisplayMD2(0);  //display frame 0
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}