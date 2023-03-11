#ifndef SQUARE_H
#define SQUARE_H

#include "includes.h"
#include "Plane.h"

class Quad : public Plane
{
public:
	virtual ~Quad() = default;

	virtual SurfaceType getSurfaceType() const override;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif