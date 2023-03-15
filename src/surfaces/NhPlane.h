#ifndef NHPLANE_H
#define NHPLANE_H

#include "includes.h"
#include "Surface.h"

class NhPlane : public Surface
{
public:
	NhPlane(const glm::vec3& origin, const glm::vec3& basis1, const glm::vec3& basis2);
	virtual ~NhPlane() = default;

	virtual bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const override;

	glm::vec3 origin;
	glm::vec3 basis1;
	glm::vec3 basis2;
};

#endif