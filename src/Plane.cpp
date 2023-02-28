#include "Plane.h"
using namespace glm;

Plane::Plane(const vec3& origin, const vec3& basis1, const vec3& basis2):
	origin(origin), basis1(basis1), basis2(basis2)
{}

bool Plane::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	const glm::vec3 normal = cross(basis1, basis2);
	const float denom = dot(ray.direction, normal);
	if (denom == 0.f)
	{
		return false;
	}
	const float t = -dot(ray.origin - origin, normal) / denom;
	if (tInterval.contains(t))
	{
		tInterval.maximum = t;
		const vec3 point = ray(t);
		const vec3 alpha = inverse(mat3(basis1, basis2, normal)) * (point - origin);
		hit = Hit(t, point, normal, basis1, -basis2, vec2(alpha.x, 1.f - alpha.y));
		return true;
	}
	return false;
}
