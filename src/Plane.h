#ifndef HALFSPACE_H
#define HALFSPACE_H

#include "includes.h"
#include "Surface.h"

class Plane : public Surface
{
public:
	virtual ~Plane() = default;

	virtual SurfaceType getSurfaceType() const override;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif