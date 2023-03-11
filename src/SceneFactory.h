#ifndef SCENEFACTORY_H
#define SCENEFACTORY_H

#include "includes.h"
#include "Scene.h"

class SceneFactory
{
public:
	static Scene* createEmptyScene();
	static Scene* createSampleScene();
};

#endif