#include "Disc.h"
#include "surfaces.h"
using namespace glm;

Surface::SurfaceType Disc::getSurfaceType() const
{
	return SurfaceType::Disc;
}

bool Disc::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	Interval<float> tempInterval(tInterval);
	Hit tempHit;
	const bool tempResult = surfaces::plane.intersect(ray, tempInterval, tempHit);
	if (tempResult && dot(tempHit.uv, tempHit.uv) <= 1.f)
	{
		tInterval = tempInterval;
		hit = tempHit;
		hit.uv = hit.uv * 0.5f + 0.5f;
		return true;
	}
	return false;
}