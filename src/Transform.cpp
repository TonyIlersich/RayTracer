#include "Transform.h"
#include "constants.h"
using namespace glm;

void Transform::scaleBy(const vec3& delta)
{
	scale *= delta;
}

void Transform::addScale(const vec3& delta)
{
	scale += delta;
}

void Transform::setScale(const vec3& scale)
{
	this->scale = scale;
}

void Transform::translate(const vec3& delta)
{
	translation += delta;
}

void Transform::setTranslation(const vec3& translation)
{
	this->translation = translation;
}

void Transform::rotate(const quat& delta)
{
	rotation = delta * rotation;
}

void Transform::setRotation(const quat& rotation)
{
	this->rotation = rotation;
}

const glm::vec3& Transform::getScale() const
{
	return scale;
}

const glm::quat& Transform::getRotation() const
{
	return rotation;
}

const vec3& Transform::getTranslation() const
{
	return translation;
}

mat4 Transform::getMatrix() const
{
	return glm::translate(toMat4(rotation) * glm::scale(identityTrans, scale), translation);
}
