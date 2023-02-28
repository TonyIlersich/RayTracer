#ifndef SPHERE_H
#define SPHERE_H

#include "includes.h"
#include "Volume.h"

class Sphere : public Volume
{
public:
	virtual ~Sphere() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif