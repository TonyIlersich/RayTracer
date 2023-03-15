#ifndef CYLINDER_H
#define CYLINDER_H

#include "includes.h"
#include "Surface.h"

class Cylinder : public Surface
{
public:
	virtual ~Cylinder() = default;

	virtual SurfaceType getSurfaceType() const override;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif