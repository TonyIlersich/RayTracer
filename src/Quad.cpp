#include "Quad.h"
using namespace glm;

Quad::Quad(const vec3 &origin, const vec3& basis1, const vec3& basis2):
	plane(origin, basis1, basis2)
{}

bool Quad::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	// TODO: consider using axis-aligned plane
	Interval<float> tempInterval(tInterval);
	Hit tempHit;
	const bool tempResult = plane.intersect(ray, tempInterval, tempHit);
	if (tempResult && tempHit.uv == clamp(tempHit.uv, vec2(0), vec2(1)))
	{
		tInterval = tempInterval;
		hit = tempHit;
		hit.uv = hit.uv * 0.5f + 0.5f;
		return true;
	}
	return false;
}
