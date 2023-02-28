#ifndef HALFSPACE_H
#define HALFSPACE_H

#include "includes.h"
#include "Volume.h"

class HalfSpace : public Volume
{
public:
	virtual ~HalfSpace() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif