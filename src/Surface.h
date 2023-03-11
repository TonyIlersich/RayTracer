#ifndef SURFACE_H
#define SURFACE_H

#include "includes.h"
#include "Hit.h"
#include "Interval.h"
#include "Ray.h"

class Surface
{
public:
	enum class SurfaceType : glm::uint
	{
		Cube,
		Cylinder,
		Disc,
		Plane,
		Quad,
		Sphere,
	};

	virtual ~Surface() = default;

	virtual SurfaceType getSurfaceType() const = 0;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const = 0;
};

#endif