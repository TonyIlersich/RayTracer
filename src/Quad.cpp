#include "Plane.h"
#include "Quad.h"
using namespace glm;

Surface::SurfaceType Quad::getSurfaceType() const
{
	return SurfaceType::Quad;
}

bool Quad::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	Interval<float> tempInterval(tInterval);
	Hit tempHit;
	const bool tempResult = Plane::intersect(ray, tempInterval, tempHit);
	if (tempResult && tempHit.uv == clamp(tempHit.uv, vec2(-1), vec2(1)))
	{
		tInterval = tempInterval;
		hit = tempHit;
		hit.uv = hit.uv * 0.5f + 0.5f;
		return true;
	}
	return false;
}
