#include "Disc.h"
using namespace glm;

Disc::Disc(const vec3& origin, const vec3& basis1, const vec3& basis2):
	plane(origin, basis1, basis2)
{}

bool Disc::intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const
{
	// TODO: consider using axis-aligned plane
	Interval<float> tempInterval(tInterval);
	Hit tempHit;
	const bool tempResult = plane.intersect(ray, tempInterval, tempHit);
	const vec3 fromCenter = tempHit.uv - vec2(0.5f);
	if (tempResult && dot(fromCenter, fromCenter) <= .25f)
	{
		tInterval = tempInterval;
		hit = tempHit;
		hit.uv = hit.uv * 0.5f + 0.5f;
		return true;
	}
	return false;
}