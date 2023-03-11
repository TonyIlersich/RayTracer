#ifndef CAMERA_H
#define CAMERA_H

#include "includes.h"
#include "Ray.h"
#include "Scene.h"
#include "constants.h"

class Eye
{
public:
	Eye(float fov, const glm::mat4& trans = identityTrans);

	// x,y = +/-1 correspond to the edges of a view square with given fov
	Ray getRay(float x, float y) const;

	void setFov(float fov);
	void setTrans(const glm::mat4& trans);
	void applyTrans(const glm::mat4& trans);

	float getFov() const;
	const glm::mat4& getTrans() const;
	const glm::mat4& getInvtrans() const;

private:
	glm::mat4 trans;
	glm::mat4 invtrans;
	float fov;

	// cached
	float edgeDist;
};

#endif