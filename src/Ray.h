#ifndef RAY_H
#define RAY_H

#include "includes.h"
#include "constants.h"

constexpr float rayEpsilon = 1e-4f;

struct Ray
{
	Ray(const glm::vec3& origin, const glm::vec3& direction, float refractiveIndex = refractiveIndexAir);

	glm::vec3 operator()(float t) const;

	void applyTransform(const glm::mat4& delta);

	glm::vec3 origin;
	glm::vec3 direction;
	float refractiveIndex;
};

Ray operator*(const glm::mat4& trans, const Ray& ray);

std::ostream& operator<<(std::ostream& out, const Ray& ray);

#endif