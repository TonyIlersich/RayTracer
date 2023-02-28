#ifndef CAMERA_H
#define CAMERA_H

#include "includes.h"
#include "Ray.h"
#include "Scene.h"

class Eye
{
public:
	Eye(const glm::vec3& origin, const glm::vec3& forward, const glm::vec3& up, const glm::vec2& fov);

	// x,y in [-1,+1]
	Ray getRay(float x, float y) const;

	void setOrigin(const glm::vec3& origin);
	void setFacing(const glm::vec3& forward, const glm::vec3& up);
	void setFov(const glm::vec2& fov);
	void transform(const glm::mat4& trans);

private:
	glm::vec3 origin;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec2 fov;

	// cached
	glm::vec3 right;
	glm::vec3 toRightEdge;
	glm::vec3 toTopEdge;
};

#endif