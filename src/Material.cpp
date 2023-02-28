#include "Material.h"

Material::Material(const EmissionInfo& emission, const ReflectionInfo& reflection, const RefractionInfo& refraction):
	emission(emission), reflection(reflection), refraction(refraction)
{}
