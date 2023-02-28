#ifndef PLANE_H
#define PLANE_H

#include "includes.h"
#include "Hittable.h"

class Plane : public Hittable
{
public:
	Plane(const glm::vec3& origin, const glm::vec3& basis1, const glm::vec3& basis2);
	virtual ~Plane() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;

	glm::vec3 origin;
	glm::vec3 basis1;
	glm::vec3 basis2;
};

#endif