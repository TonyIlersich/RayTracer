#include "EyeController.h"
using namespace glm;

EyeController::EyeController():
	eye(nullptr),
	position(0.f),
	rotation(0.f),
	sensitivity(1.f)
{}

void EyeController::setEye(Eye* eye)
{
	this->eye = eye;
}

void EyeController::setSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}

void EyeController::moveCamera(const vec3& step, const vec2& turn)
{
	rotation += turn * sensitivity * tau;
	rotation.x = mod(rotation.x, tau);
	rotation.y = clamp(rotation.y, -tau * 0.25f, tau * 0.25f);
	const mat4 xrot = rotate(identityTrans, rotation.x, upward);
	const mat4 yrot = rotate(identityTrans, rotation.y, rightward);
	position += mat3(xrot) * step;
	eye->setTrans(translate(identityTrans, position) * xrot * yrot);
}
