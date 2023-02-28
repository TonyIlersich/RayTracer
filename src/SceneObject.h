#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "includes.h"
#include "Interval.h"
#include "Volume.h"
#include "Material.h"

class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const;

	const Material* getMaterial() const;

	void applyTransform(glm::mat4 delta);
	void addChild(SceneObject* child);
	void setVolume(Volume* newShape);
	void setMaterial(Material* newMaterial);

protected:
	glm::mat4 trans; // from local space to parent space
	glm::mat4 invtrans; // from parent space to local space
	std::vector<SceneObject*> children; // ownership
	Volume* volume; // non-ownership
	Material* material; // non-ownership
};

#endif