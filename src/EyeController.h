#ifndef EYECONTROLLER_H
#define EYECONTROLLER_H

#include "includes.h"
#include "Eye.h"

class EyeController
{
public:
	void setEye(Eye* eye);
	void setSensitivity(float sensitivity);
	void moveCamera(const glm::vec3& step, const glm::vec2& turn);

private:
	Eye* eye;
	glm::vec3 position;
	glm::vec2 rotation;
	float sensitivity;
};

#endif