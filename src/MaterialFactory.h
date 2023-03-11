#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "includes.h"
#include "Material.h"

constexpr size_t maxMaterials = 128;

class MaterialFactory
{
public:
	static const Material* createMaterial(
		const glm::vec3& diffuseEmission,
		const glm::vec3& directedEmission,
		const glm::vec3& albedo,
		float refractivity,
		float refractiveIndex,
		float glossiness);
	static const Material* createMatte(
		const glm::vec3& albedo);
	static const Material* createEmissive(
		const glm::vec3& diffuseEmission,
		const glm::vec3& directedEmission = white);
	static const Material* createReflective(
		const glm::vec3& albedo = white,
		float glossiness = 0.f);
	static const Material* createRefractive(
		float refractiveIndex,
		const glm::vec3& albedo = white,
		float glossiness = 0.f);
	static const Material* createDielectric(
		float refractiveIndex,
		float refractivity,
		const glm::vec3& albedo = white,
		float glossiness = 0.f);

private:
	static Material materials[maxMaterials];
	static size_t materialCount;
};

#endif
