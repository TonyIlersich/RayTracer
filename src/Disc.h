#ifndef DISC_H
#define DISC_H

#include "includes.h"
#include "Plane.h"

class Disc : public Hittable
{
public:
	Disc(const glm::vec3& origin, const glm::vec3& basis1, const glm::vec3& basis2);
	virtual ~Disc() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;

	Plane plane;
};

#endif