#include "Eye.h"
#include "Interval.h"
using namespace std;
using namespace glm;

Eye::Eye(float fov, const mat4& trans):
	trans(trans),
	invtrans(inverse(trans)),
	fov(fov),
	edgeDist(tan(fov * 0.5f))
{}

Ray Eye::getRay(float x, float y) const
{
	return trans * Ray(vec3(0.f), vec3(x * edgeDist, y * edgeDist, -1.f));
}

void Eye::setFov(float fov)
{
	this->fov = fov;
}

void Eye::setTrans(const mat4& trans)
{
	this->trans = trans;
	invtrans = inverse(trans);
}

void Eye::applyTrans(const mat4& trans)
{
	setTrans(trans * this->trans);
}

float Eye::getFov() const
{
	return fov;
}

const mat4& Eye::getTrans() const
{
	return trans;
}

const mat4& Eye::getInvtrans() const
{
	return invtrans;
}
