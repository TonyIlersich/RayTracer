#include "Cylinder.h"
#include "surfaces.h"
using namespace glm;

static const mat4 capTrans[2]{
	inverse(translate(identityTrans, upward)), // top
	inverse(translate(scale(identityTrans, vec3(1,-1,1)), upward)), // bottom
};

Surface::SurfaceType Cylinder::getSurfaceType() const
{
	return SurfaceType::Cylinder;
}

bool Cylinder::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	bool result = false;
	// Check caps
	for (auto& trans : capTrans)
	{
		const Ray localRay = trans * ray;
		if(surfaces::disc.intersect(localRay, tInterval, hit))
		{
			result = true;
			hit.applyTransform(trans);
		}
	}
	// Check round side
	const float o2 = ray.origin.x    * ray.origin.x    + ray.origin.z    * ray.origin.z;
	const float d2 = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
	const float od = ray.origin.x    * ray.direction.x + ray.origin.z    * ray.direction.z;
	const float discriminant = od * od - d2 * (o2 - 1.f);
	if (discriminant < 0.f)
	{
		return result;
	}
	const float rootDisc = sqrt(discriminant);
	float t = INFINITY;
	const Interval<float> heightInterval(-1.f, 1.f);
	const float t1 = (-od + rootDisc) / d2;
	if (tInterval.contains(t1) && heightInterval.contains(ray(t1).y))
	{
		t = tInterval.maximum = t1;
	}
	const float t2 = (-od - rootDisc) / d2;
	if (tInterval.contains(t2) && heightInterval.contains(ray(t2).y))
	{
		t = tInterval.maximum = t2;
	}
	if (isinf(t))
	{
		return result;
	}
	const vec3 point = ray(t);
	const vec3 normal = vec3(point.x, 0, point.z);
	const vec3 u = vec3(point.z, 0, -point.x);
	const vec3 v = downward;
	const float phi = atan(normal.x, normal.z);
	hit = Hit(t, point, normal, u, v, vec2(phi / tau + .5f, -point.y));
	return true;
}
