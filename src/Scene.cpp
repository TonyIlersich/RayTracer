#include "Scene.h"
#include "utils.h"
using namespace std;
using namespace glm;

// #define DEBUG_RAYS

constexpr int maxDepth = 10;
constexpr vec3 minSignificance(1e-3f);
constexpr int glossSampleCount = 20;

bool Scene::intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const
{
	return root.intersect(ray, tInterval, hit);
}

vec3 Scene::getRayColor(const Ray& ray) const
{
	return getRayColorRec(ray, 0, vec3(1.f));
}

vec3 Scene::getRayColorRec(const Ray& ray, int depth, const vec3& significance) const
{
	if (depth == maxDepth || all(lessThan(significance, minSignificance)))
	{
		return vec3(0);
	}

#ifdef DEBUG_RAYS
	const string indent(depth, '\t');
	cerr << indent << "Depth: " << depth << ", ray: " << ray << '\n';
#endif

	Hit hit;
	Interval<float> tInterval(rayEpsilon, INFINITY);
	if (intersect(ray, tInterval, hit))
	{
#ifdef DEBUG_RAYS
		cerr << indent << "hit at " << hit.point << " with normal: " << hit.normal << '\n';
#endif
		assert(hit.sceneObject != nullptr);

		const Material* mat = hit.sceneObject->getMaterial();
		if (mat == nullptr)
		{
			return vec3(1, 0, 1);
		}

		hit.normal = normalize(hit.normal);
		hit.u = normalize(hit.u);
		hit.v = normalize(hit.v);

		glm::vec3 color = vec3(0);

		const float viewAngleAttenuation = abs(dot(ray.direction, hit.normal)) / (length(ray.direction) * length(hit.normal));

		// Emissive contribution
		color += mat->emission.diffuse + mat->emission.directed * viewAngleAttenuation;

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
					const float p = i + rayRandom.getValue();
					const float polarAngle = acos(1.f - p / glossSampleCount);
					const float azimuthalAngle = azimuthalAngleStep * p;
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

		return color;
	}
	else
	{
		return vec3(0);
		// return vec3(0, 0.5f * dot(ray.direction, vec3(0,-1,0)), 0.2f + 0.3f * dot(ray.direction, vec3(0,1,0)));
	}
}
