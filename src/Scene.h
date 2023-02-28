#ifndef SCENE_H
#define SCENE_H

#include "includes.h"
#include "Interval.h"
#include "Random.h"
#include "Ray.h"
#include "SceneObject.h"

class Scene
{
public:
	bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const;

	glm::vec3 getRayColor(const Ray& ray) const;

	SceneObject root;

private:
	glm::vec3 getRayColorRec(const Ray& ray, int maxDepth, const glm::vec3& significance) const;

	mutable Random rayRandom;
};

#endif