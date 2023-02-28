#ifndef RAYHIT_H
#define RAYHIT_H

#include "includes.h"
#include "constants.h"
#include "Ray.h"

class SceneObject;

struct Hit
{
	Hit();
	Hit(float t, const glm::vec3& point, const glm::vec3& normal, const glm::vec3& u, const glm::vec3& v, const glm::vec2& uv);

	operator bool() const;

	void applyTransform(const glm::mat4& delta);

	float t;
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 u, v;
	glm::vec2 uv;
	const SceneObject* sceneObject;
};

#endif