#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "includes.h"
#include "Interval.h"
#include "surfaces/Surface.h"
#include "Material.h"

class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const;

	const glm::mat4& getTrans() const;
	const glm::mat4& getInvtrans() const;
	const std::vector<SceneObject*>& getChildren() const;
	const Surface* getSurface() const;
	const Material* getMaterial() const;

	void applyTransform(glm::mat4 delta);
	void addChild(SceneObject* child);
	void setSurface(const Surface* newSurface);
	void setMaterial(const Material* newMaterial);

protected:
	glm::mat4 trans; // from local space to parent space
	glm::mat4 invtrans; // from parent space to local space
	std::vector<SceneObject*> children; // ownership
	const Surface* surface; // non-ownership
	const Material* material; // non-ownership
};

#endif