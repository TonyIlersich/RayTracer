#ifndef DISC_H
#define DISC_H

#include "includes.h"
#include "Surface.h"

class Disc : public Surface
{
public:
	virtual ~Disc() = default;

	virtual SurfaceType getSurfaceType() const override;
	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;
};

#endif