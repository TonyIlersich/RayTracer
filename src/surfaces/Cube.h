#ifndef CUBE_H
#define CUBE_H

#include "includes.h"
#include "Surface.h"

class Cube : public Surface
{
public:
	virtual ~Cube() = default;

	virtual SurfaceType getSurfaceType() const override;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif