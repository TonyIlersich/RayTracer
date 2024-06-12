#include "MaterialFactory.h"
#include "SceneFactory.h"
#include "surfaces/surfaces.h"
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

	const Material* sunlight   = MaterialFactory::createEmissive(10'000.f * white);
	const Material* mirror     = MaterialFactory::createReflective();
	const Material* glass      = MaterialFactory::createDielectric(1.1f, 0.99f);
	const Material* tile       = MaterialFactory::createMatte(gray);
	const Material* redLaser   = MaterialFactory::createMaterial(5.f * red, black, white, 1.f, refractiveIndexAir, 0.f);
	const Material* greenLaser = MaterialFactory::createMaterial(5.f * blue, black, white, 1.f, refractiveIndexAir, 0.f);
	const Material* blueLaser  = MaterialFactory::createMaterial(5.f * green, black, white, 1.f, refractiveIndexAir, 0.f);
	const Material* glowing    = MaterialFactory::createEmissive(1.f * yellow);

	SceneObject* sun = new SceneObject();
	sun->setSurface(&surfaces::sphere);
	sun->setMaterial(sunlight);
	sun->applyTransform(scale(identityTrans, vec3(100, 100, 100)));
	sun->applyTransform(translate(identityTrans, vec3(-200, 1000, 0)));
	scene->root.addChild(sun);

	SceneObject* mirrorObject = new SceneObject();
	mirrorObject->setSurface(&surfaces::quad);
	mirrorObject->setMaterial(mirror);
	mirrorObject->applyTransform(scale(identityTrans, vec3(2.f, 1.f, 4.f)));
	mirrorObject->applyTransform(rotate(identityTrans, tau * .28f, vec3(1,0,0)));
	mirrorObject->applyTransform(rotate(identityTrans, tau * .25f, vec3(0,1,0)));
	mirrorObject->applyTransform(translate(identityTrans, vec3(-4.f, 3.f, -10.f)));
	scene->root.addChild(mirrorObject);

	SceneObject* glassBall = new SceneObject();
	glassBall->setSurface(&surfaces::sphere);
	glassBall->setMaterial(glass);
	glassBall->applyTransform(translate(identityTrans, vec3(0,1.5f,-9)));
	scene->root.addChild(glassBall);

	SceneObject* redLaserBeam = new SceneObject();
	redLaserBeam->setSurface(&surfaces::cylinder);
	redLaserBeam->setMaterial(redLaser);
	redLaserBeam->applyTransform(scale(identityTrans, vec3(0.2f, 2.f, 0.2f)));
	redLaserBeam->applyTransform(translate(identityTrans, vec3(2.f, 5.f, -10.f)));
	scene->root.addChild(redLaserBeam);

	SceneObject* greenLaserBall = new SceneObject();
	greenLaserBall->setSurface(&surfaces::sphere);
	greenLaserBall->setMaterial(greenLaser);
	greenLaserBall->applyTransform(translate(identityTrans, vec3(-1.5f, 5.f, -9.f)));
	scene->root.addChild(greenLaserBall);

	SceneObject* blueLaserBall = new SceneObject();
	blueLaserBall->setSurface(&surfaces::sphere);
	blueLaserBall->setMaterial(blueLaser);
	blueLaserBall->applyTransform(translate(identityTrans, vec3(0,5,-8)));
	scene->root.addChild(blueLaserBall);

	SceneObject* glowCube = new SceneObject();
	glowCube->setSurface(&surfaces::cube);
	glowCube->setMaterial(glowing);
	glowCube->applyTransform(rotate(identityTrans, tau / 12.f, vec3(0,1,0)));
	glowCube->applyTransform(translate(identityTrans, vec3(5.f, 2.f, -10.f)));
	scene->root.addChild(glowCube);

	SceneObject* glassCube = new SceneObject();
	glassCube->setSurface(&surfaces::cube);
	glassCube->setMaterial(glass);
	glassCube->applyTransform(scale(identityTrans, vec3(2.f, 2.f, 0.5f)));
	glassCube->applyTransform(translate(identityTrans, vec3(3.f, 2.5f, -5.f)));
	scene->root.addChild(glassCube);

	SceneObject* ground = new SceneObject();
	ground->setSurface(&surfaces::plane);
	ground->setMaterial(tile);
	scene->root.addChild(ground);

	return scene;
}
