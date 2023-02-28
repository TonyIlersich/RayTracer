#ifndef MATERIAL_H
#define MATERIAL_H

#include "includes.h"
#include "colors.h"
#include "Ray.h"
#include "Hit.h"

class Material
{
public:

	struct EmissionInfo
	{
		glm::vec3 diffuse = glm::vec3(0);
		glm::vec3 directed = glm::vec3(0);
	};

	struct ReflectionInfo
	{
		glm::vec3 strength = glm::vec3(0);
		float glossiness = 0.0f;
	};

	struct RefractionInfo
	{
		glm::vec3 strength = glm::vec3(0);
		float glossiness = 0.0f;
		float index = refractiveIndexAir;
	};

	Material(
		const EmissionInfo& emission = { black, black },
		const ReflectionInfo& reflection = { black, 0.f },
		const RefractionInfo& refraction = { black, 0.f, refractiveIndexAir });

	EmissionInfo emission;
	ReflectionInfo reflection;
	RefractionInfo refraction;
};

#endif