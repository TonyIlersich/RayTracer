#ifndef SQUARE_H
#define SQUARE_H

#include "includes.h"
#include "Plane.h"

class Quad : public Hittable
{
public:
	Quad(const glm::vec3& origin, const glm::vec3& basis1, const glm::vec3& basis2);
	virtual ~Quad() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;

	Plane plane;
};

#endif