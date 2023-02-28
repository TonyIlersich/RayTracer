#ifndef CUBE_H
#define CUBE_H

#include "includes.h"
#include "Volume.h"

class Cube : public Volume
{
public:
	virtual ~Cube() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif