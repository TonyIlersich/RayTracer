#include "MaterialFactory.h"
#include "SceneFactory.h"
#include "surfaces.h"
#include "utils.h"
using namespace std;
using namespace glm;

Scene* SceneFactory::createEmptyScene()
{
	return new Scene();
}

Scene* SceneFactory::createSampleScene()
{
	Scene* scene = new Scene();

	const Material* mirror     = MaterialFactory::createReflective();
	const Material* glass      = MaterialFactory::createDielectric(1.3f, 0.9f, white, 0.1f);
	const Material* tile       = MaterialFactory::createMatte(gray);
	const Material* redLaser   = MaterialFactory::createMaterial(10.f * red,   0.5f * white, white, 1.f, refractiveIndexAir, 0.f);
	const Material* greenLaser = MaterialFactory::createMaterial(10.f * blue, 0.5f * white, white, 1.f, refractiveIndexAir, 0.f);
	const Material* blueLaser  = MaterialFactory::createMaterial(10.f * green,  0.5f * white, white, 1.f, refractiveIndexAir, 0.f);
	const Material* glowing    = MaterialFactory::createEmissive(black, yellow);

	SceneObject* mirrorObject = new SceneObject();
	mirrorObject->setSurface(&surfaces::cylinder);
	mirrorObject->setMaterial(mirror);
	mirrorObject->applyTransform(scale(identityTrans, vec3(1.5f,0.2f,2.5)));
	mirrorObject->applyTransform(rotate(identityTrans, tau *  0.26f, vec3(0,0,1)));
	mirrorObject->applyTransform(rotate(identityTrans, tau * -0.10f, vec3(0,1,0)));
	mirrorObject->applyTransform(translate(identityTrans, vec3(-3.5f,1.75f,-10)));
	scene->root.addChild(mirrorObject);

	SceneObject* glassBall = new SceneObject();
	glassBall->setSurface(&surfaces::sphere);
	glassBall->setMaterial(glass);
	glassBall->applyTransform(translate(identityTrans, vec3(0,1.5f,-9)));
	scene->root.addChild(glassBall);

	SceneObject* redLaserBall = new SceneObject();
	redLaserBall->setSurface(&surfaces::sphere);
	redLaserBall->setMaterial(redLaser);
	redLaserBall->applyTransform(translate(identityTrans, vec3(1.5f,5,-10)));
	scene->root.addChild(redLaserBall);

	SceneObject* greenLaserBall = new SceneObject();
	greenLaserBall->setSurface(&surfaces::sphere);
	greenLaserBall->setMaterial(greenLaser);
	greenLaserBall->applyTransform(translate(identityTrans, vec3(-1.5f,5,-10)));
	scene->root.addChild(greenLaserBall);

	SceneObject* blueLaserBall = new SceneObject();
	blueLaserBall->setSurface(&surfaces::sphere);
	blueLaserBall->setMaterial(blueLaser);
	blueLaserBall->applyTransform(translate(identityTrans, vec3(0,5,-8)));
	scene->root.addChild(blueLaserBall);

	SceneObject* glowCube = new SceneObject();
	glowCube->setSurface(&surfaces::cube);
	glowCube->setMaterial(glowing);
	glowCube->applyTransform(rotate(identityTrans, tau / 24.f, vec3(0,1,0)));
	glowCube->applyTransform(translate(identityTrans, vec3(4,1.25f,-10)));
	scene->root.addChild(glowCube);

	SceneObject* ground = new SceneObject();
	ground->setSurface(&surfaces::plane);
	ground->setMaterial(tile);
	scene->root.addChild(ground);

	return scene;
}
