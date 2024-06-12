#include "Scene.h"
#include "utils.h"
using namespace std;
using namespace glm;

// #define DEBUG_RAYS
// #define UNLIT

#ifdef UNLIT
constexpr int maxDepth = 1;
constexpr vec3 minSignificance(1e-3f);
#else
constexpr int maxDepth = 10;
constexpr vec3 minSignificance(1e-3f);
#endif

bool Scene::intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const
{
	return root.intersect(ray, tInterval, hit);
}

vec3 Scene::getRayColor(const Ray& ray) const
{
	return getRayColorRec(ray, 0, vec3(1.f));
}

void Scene::serializeForShader(
	mat4* worldToLocal, GLuint* surfaceType,
	GLuint* surfaceMaterialId, vec4* materialEmission,
	vec4* materialAlbedo, float* materialGlossiness,
	GLuint& surfaceCount,
	unordered_map<const Material*, GLuint>& materialIdTable) const
{
	surfaceCount = 0;
	serializeForShaderRec(
		worldToLocal, surfaceType, surfaceMaterialId, materialEmission, materialAlbedo, materialGlossiness, surfaceCount,
		materialIdTable,
		identityTrans, &root);
}

vec3 Scene::getRayColorRec(const Ray& ray, int depth, const vec3& significance) const
{
	if (depth == maxDepth || all(lessThan(significance, minSignificance)))
	{
		return vec3(0);
	}

#ifdef DEBUG_RAYS
	const string indent(depth + 1, '\t');
	cerr << indent << "Depth: " << depth << ", ray: " << ray << '\n';
#endif

	Hit hit;
	Interval<float> tInterval(rayEpsilon, INFINITY);
	if (intersect(ray, tInterval, hit))
	{
#ifdef DEBUG_RAYS
		cerr << indent << "Hit at " << hit.point << " with normal: " << hit.normal << '\n';
#endif
		assert(hit.sceneObject != nullptr);

		const Material* mat = hit.sceneObject->getMaterial();
		if (mat == nullptr)
		{
			return vec3(1, 0, 1);
		}

#ifdef DEBUG_RAYS
		cerr << indent << "Material: " << *mat << '\n';
#endif

		hit.normal = normalize(hit.normal);
		hit.u = normalize(hit.u);
		hit.v = normalize(hit.v);

		vec3 color = black;

		// Emissive contribution
		const float viewAngleAttenuation = abs(dot(ray.direction, hit.normal)) / length(ray.direction);
		color += mat->diffuseEmission + mat->directedEmission * viewAngleAttenuation;

#ifdef UNLIT
		return color + mat->albedo;
#endif

		if (mat->albedo != black)
		{
			// Probabilistic indirect lighting
			bool didRefract = false;
			if (rayRandom.getValue() < mat->refractivity)
			{
#ifdef DEBUG_RAYS
				cerr << indent << "Trying to refract\n";
#endif
				// Refract
				// TODO: how to handle refractive indices with overlapping objects?
				bool isEntering = dot(ray.direction, hit.normal) < 0;
				const float newRefractiveIndex = isEntering ? mat->refractiveIndex : refractiveIndexAir;
				vec3 bounceDir;
				if (rayRandom.getValue() < mat->glossiness)
				{
#ifdef DEBUG_RAYS
					cerr << indent << "Glossy\n";
#endif
					// Glossy bounce
					const vec3 local = rayRandom.getUnitVec3();
					bounceDir = 
						(isEntering ? -hit.normal : hit.normal) * abs(local.y) +
						hit.u * local.x +
						hit.v * local.z;
				}
				else
				{
#ifdef DEBUG_RAYS
					cerr << indent << "Perfect\n";
#endif
					// Perfect bounce
					const float eta = ray.refractiveIndex / newRefractiveIndex;
					bounceDir = refract(ray.direction, isEntering ? hit.normal : -hit.normal, eta);
				}
				if (bounceDir != vec3(0))
				{
#ifdef DEBUG_RAYS
					cerr << indent << "Refracting\n";
#endif
					color += mat->albedo * getRayColorRec(
						Ray(hit.point, bounceDir, newRefractiveIndex),
						depth + 1,
						mat->albedo * significance);
					didRefract = true;
				}
#ifdef DEBUG_RAYS
				else
				{
					cerr << indent << "Refraction failed: total internal reflection\n";
				}
#endif
			}
			if (!didRefract)
			{
#ifdef DEBUG_RAYS
				cerr << indent << "Trying to reflect\n";
#endif
				// Reflect
				vec3 bounceDir;
				if (rayRandom.getValue() < mat->glossiness)
				{
#ifdef DEBUG_RAYS
					cerr << indent << "Glossy\n";
#endif
					// Glossy bounce
					const vec3 local = rayRandom.getUnitVec3();
					bounceDir = 
						hit.normal * abs(local.y) +
						hit.u * local.x +
						hit.v * local.z;
				}
				else
				{
#ifdef DEBUG_RAYS
					cerr << indent << "Perfect\n";
#endif
					// Perfect bounce
					bounceDir = reflect(ray.direction, hit.normal);
				}
#ifdef DEBUG_RAYS
				cerr << indent << "Reflecting\n";
#endif
				color += mat->albedo * getRayColorRec(
					Ray(hit.point, bounceDir, ray.refractiveIndex),
					depth + 1,
					mat->albedo * significance);
			}
		}



		/* OLD: Branching reflections
		// Refractive contribution
		bool totalInternalReflection = false;
		if (mat->refraction.strength != vec3(0.f))
		{
			// TODO: what about overlapping objects?
			bool isEntering = dot(ray.direction, hit.normal) < 0;
			const float newRefractiveIndex = isEntering ? mat->refraction.index : refractiveIndexAir;
			const float eta = ray.refractiveIndex / newRefractiveIndex;
			// cerr << ray.refractiveIndex << " --|-> " << newRefractiveIndex << " = " << eta << '\n';
			vec3 bounceDir = refract(ray.direction, isEntering ? hit.normal : -hit.normal, eta);
			if (bounceDir == vec3(0))
			{
#ifdef DEBUG_RAYS
				cerr << indent << "Total internal reflection\n";
#endif
				totalInternalReflection = true;
			}
			else
			{
#ifdef DEBUG_RAYS
				cerr << indent << "Refracting\n";
#endif
				// TODO
				// if (mat->reflection.glossiness > 0.0f)
				// {
				// 	const vec2 gloss = mat->reflection.glossiness * rayRandom.getInUnitCircle();
				// 	bounceDir += hit.u * gloss.x + hit.v * gloss.y;
				// }
				color += mat->refraction.strength * getRayColorRec(
					Ray(hit.point, bounceDir, newRefractiveIndex),
					depth + 1,
					mat->refraction.strength * significance);
			}
		}

		// Reflective contribution
		const vec3 reflectivity = mat->reflection.strength + (totalInternalReflection ? mat->refraction.strength : vec3(0.f));
		if (reflectivity != vec3(0.f))
		{
#ifdef DEBUG_RAYS
			cerr << indent << "Reflecting\n";
#endif
			// Diffuse reflection
			if (mat->reflection.glossiness > 0.f)
			{
				constexpr float azimuthalAngleStep = tau * goldenRatio;
				constexpr float maxAngle = tau / 4.f;
				const vec3 strength = reflectivity / float(glossSampleCount);
				for (int i = 0; i < glossSampleCount; i++)
				{
					const float polarAngle = acos(1.f - (i + rayRandom.getValue()) / glossSampleCount);
					const float azimuthalAngle = azimuthalAngleStep * (i + rayRandom.getValue());
					const vec3 thisBounceDir =
						hit.normal * cos(polarAngle) +
						hit.u * sin(polarAngle) * cos(azimuthalAngle) +
						hit.v * sin(polarAngle) * sin(azimuthalAngle);
					color += strength * getRayColorRec(
						Ray(hit.point, thisBounceDir, ray.refractiveIndex),
						depth + 1,
						strength * significance);
				}
			}
			// Perfect reflection
			if (mat->reflection.glossiness < 1.f)
			{
				const vec3 bounceDir = reflect(ray.direction, hit.normal);
				const vec3 strength = (1.f - mat->reflection.glossiness) * reflectivity;
				color += strength * getRayColorRec(
						Ray(hit.point, bounceDir, ray.refractiveIndex),
						depth + 1,
						strength * significance);
			}
		}
		/* */

#ifdef DEBUG_RAYS
		cerr << indent << "Ray color: " << color << '\n';
#endif
		return color;
	}
	else
	{
#ifdef DEBUG_RAYS
		cerr << indent << "Hit nothing\n";
		return vec3(0);
#else
		return vec3(0, 0.2f * dot(ray.direction, upward), 0.1f + 0.1f * dot(ray.direction, upward));
#endif
	}
}

void Scene::serializeForShaderRec(
	mat4* worldToLocal, GLuint* surfaceType,
	GLuint* surfaceMaterialId, vec4* materialEmission,
	vec4* materialAlbedo, float* materialGlossiness,
	GLuint& surfaceCount,
	unordered_map<const Material*, GLuint>& materialIdTable,
	const mat4& currentWorldToLocal, const SceneObject* sceneObject) const
{
	// cerr << '[';
	const mat4 thisWorldToLocal = sceneObject->getInvtrans() * currentWorldToLocal;
	if (sceneObject->getSurface() != nullptr)
	{
		// cerr << '+';
		worldToLocal[surfaceCount] = thisWorldToLocal;
		surfaceType[surfaceCount] = (GLuint)sceneObject->getSurface()->getSurfaceType();
		const Material* material = sceneObject->getMaterial();
		auto it = materialIdTable.find(material);
		if (it == materialIdTable.end())
		{
			GLuint newMaterialId = (GLuint)materialIdTable.size();
			materialIdTable[material] = newMaterialId;
			surfaceMaterialId[surfaceCount] = newMaterialId;
			materialEmission[newMaterialId] = vec4(material->diffuseEmission, material->refractiveIndex);
			materialAlbedo[newMaterialId] = vec4(material->albedo, 1.f - material->refractivity);
			materialGlossiness[newMaterialId] = material->glossiness;
		}
		else
		{
			surfaceMaterialId[surfaceCount] = it->second;
		}
		surfaceCount++;
	}
	for (const auto& c : sceneObject->getChildren())
	{
		serializeForShaderRec(
			worldToLocal, surfaceType, surfaceMaterialId, materialEmission, materialAlbedo, materialGlossiness, surfaceCount,
			materialIdTable,
			thisWorldToLocal, c);
	}
	// cerr << ']';
}
