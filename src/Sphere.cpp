#include "Sphere.h"
#include "utils.h"
using namespace std;
using namespace glm;

bool Sphere::intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const
{
	const float o2 = dot(ray.origin, ray.origin);
	const float d2 = dot(ray.direction, ray.direction);
	const float od = dot(ray.origin, ray.direction);
	const float discriminant = od * od - d2 * (o2 - 1.f);
	if (discriminant < 0.f)
	{
		return false;
	}
	const float rootDisc = sqrt(discriminant);
	const float t1 = (-od + rootDisc) / d2;
	float t = INFINITY;
	if (tInterval.contains(t1))
	{
		t = tInterval.maximum = t1;
	}
	const float t2 = (-od - rootDisc) / d2;
	if (tInterval.contains(t2))
	{
		t = tInterval.maximum = t2;
	}
	if (isinf(t))
	{
		return false;
	}
	const vec3 normal = ray(t);
	const vec3 u = normalize(cross(vec3(-normal.x, 0.f, -normal.z), normal));
	const vec3 v = cross(u, normal);
	const float phi = atan(normal.x, normal.z); // horizontal
	const float theta = acos(normal.y); // vertical
	hit = Hit(t, normal, normal, u, v, vec2(phi / tau + .5f, theta / (0.5f * tau) + .5f));
	return true;
}
