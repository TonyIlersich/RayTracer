#include "HalfSpace.h"
#include "utils.h"
using namespace std;
using namespace glm;

bool HalfSpace::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	if (ray.direction.y == 0.f)
	{
		return false;
	}
	const float t = -ray.origin.y / ray.direction.y;
	if (tInterval.contains(t))
	{
		tInterval.maximum = t;
		const vec3& point = ray(t);
		hit = Hit(t, point, vec3(0,1,0), vec3(1,0,0), vec3(0,0,1), vec2(point.x, point.z));
		return true;
	}
	return false;
}