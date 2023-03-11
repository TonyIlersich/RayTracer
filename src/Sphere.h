#ifndef SPHERE_H
#define SPHERE_H

#include "includes.h"
#include "Surface.h"

class Sphere : public Surface
{
public:
	virtual ~Sphere() = default;

	virtual SurfaceType getSurfaceType() const override;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif