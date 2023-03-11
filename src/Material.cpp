#include "Material.h"
#include "utils.h"
using namespace std;
using namespace glm;

Material::Material(
	const vec3& diffuseEmission,
	const vec3& directedEmission,
	const vec3& albedo,
	float refractivity,
	float refractiveIndex,
	float glossiness):
	diffuseEmission(diffuseEmission),
	directedEmission(directedEmission),
	albedo(albedo),
	refractivity(refractivity),
	refractiveIndex(refractiveIndex),
	glossiness(glossiness)
{}

ostream& operator<<(ostream& out, const Material& mat)
{
	return out
		<< "[E: " << mat.diffuseEmission << " + " << mat.directedEmission
		<< ", A: " << mat.albedo
		<< ", r: " << mat.refractivity
		<< ", i: " << mat.refractiveIndex
		<< ", g: " << mat.glossiness << ']';
}
