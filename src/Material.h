#ifndef MATERIAL_H
#define MATERIAL_H

#include "includes.h"
#include "colors.h"
#include "Ray.h"
#include "Hit.h"

class Material
{
public:
	Material() = default;
	Material(
		const glm::vec3& diffuseEmission,
		const glm::vec3& directedEmission,
		const glm::vec3& albedo,
		float refractivity,
		float refractiveIndex,
		float glossiness);

	glm::vec3 diffuseEmission;
	glm::vec3 directedEmission;
	glm::vec3 albedo;
	float refractivity;
	float refractiveIndex;
	float glossiness;
};

std::ostream& operator<<(std::ostream& out, const Material& mat);

#endif