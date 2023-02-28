#ifndef CYLINDER_H
#define CYLINDER_H

#include "includes.h"
#include "Volume.h"

class Cylinder : public Volume
{
public:
	virtual ~Cylinder() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif