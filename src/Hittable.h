#ifndef HITTABLE_H
#define HITTABLE_H

#include "includes.h"
#include "Hit.h"
#include "Interval.h"
#include "Ray.h"

class Hittable
{
public:
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const = 0;
};

#endif