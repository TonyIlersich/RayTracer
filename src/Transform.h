#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "includes.h"

class Transform
{
public:
	void scaleBy(const glm::vec3& factor);
	void addScale(const glm::vec3& delta);
	void setScale(const glm::vec3& scale);
	void translate(const glm::vec3& delta);
	void setTranslation(const glm::vec3& translation);
	void rotate(const glm::quat& delta);
	void setRotation(const glm::quat& rotation);

	const glm::vec3& getScale() const;
	const glm::quat& getRotation() const;
	const glm::vec3& getTranslation() const;
	glm::mat4 getMatrix() const;

private:
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
};

#endif
