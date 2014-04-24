#include "LevelFactoryImplementation.h"


LevelFactoryImplementation::LevelFactoryImplementation(void)
{
}


LevelFactoryImplementation::~LevelFactoryImplementation(void)
{
}

Level* LevelFactoryImplementation::createLevel(int levelType, float x, float y, float z){
	switch(levelType){
	case Normal: return new LevelNormal(x,y,z);
	case Enemy: return new EnemyLevel(x,y,z);
	case Ice: return new IceLevel(x,y,z);
	case Pickup: return new PickupLevel(x,y,z);
	default: return NULL;
	}
}