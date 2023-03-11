#include "MaterialFactory.h"
using namespace std;
using namespace glm;

Material MaterialFactory::materials[maxMaterials];
size_t MaterialFactory::materialCount = 0;

const Material* MaterialFactory::createMaterial(
	const vec3& diffuseEmission,
	const vec3& directedEmission,
	const vec3& albedo,
	float refractivity,
	float refractiveIndex,
	float glossiness)
{
	if (materialCount == maxMaterials)
	{
		throw runtime_error("Exceeded maximum number of materials in MaterialFactory");
	}
	materials[materialCount++] = Material(
		diffuseEmission, directedEmission, albedo,
		refractivity, refractiveIndex, glossiness);
	return &materials[materialCount - 1];
}

const Material* MaterialFactory::createMatte(const vec3& albedo)
{
	return createMaterial(black, black, albedo, 0.f, refractiveIndexAir, 1.f);
}

const Material* MaterialFactory::createEmissive(const vec3& diffuse, const vec3& directed)
{
	return createMaterial(diffuse, directed, black, 0.f, refractiveIndexAir, 1.f);
}

const Material* MaterialFactory::createReflective(const vec3& albedo, float glossiness)
{
	return createMaterial(black, black, albedo, 0.f, refractiveIndexAir, glossiness);
}

const Material* MaterialFactory::createRefractive(float refractiveIndex, const vec3& albedo, float glossiness)
{
	return createMaterial(black, black, albedo, 1.f, refractiveIndex, glossiness);
}

const Material* MaterialFactory::createDielectric(
	float refractiveIndex, float refractivity,
	const vec3& albedo, float glossiness)
{
	return createMaterial(black, black, albedo, refractivity, refractiveIndex, glossiness);	
}
