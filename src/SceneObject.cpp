#include "SceneObject.h"
#include "utils.h"
using namespace std;
using namespace glm;

SceneObject::SceneObject():
	trans(mat4(1)), invtrans(mat4(1)), volume(nullptr)
{}

SceneObject::~SceneObject()
{
	for (auto& c : children)
	{
		delete c;
	}
}

bool SceneObject::intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const
{
	const Ray localRay = invtrans * ray;
	if (volume != nullptr && volume->intersect(localRay, tInterval, hit))
	{
		hit.sceneObject = this;
		hit.applyTransform(invtrans);
		return true;
	}
	bool result = false;
	for (auto& c : children)
	{
		result |= c->intersect(localRay, tInterval, hit);
	}
	if (result)
	{
		hit.applyTransform(invtrans);
	}
	return result;
}

const Material* SceneObject::getMaterial() const
{
	return material;
}

void SceneObject::applyTransform(glm::mat4 delta)
{
	trans = delta * trans;
	invtrans = inverse(trans);
}

void SceneObject::addChild(SceneObject* child)
{
	if (child != nullptr)
	{
		children.push_back(child);
	}
}

void SceneObject::setVolume(Volume* newShape)
{
	volume = newShape;
}

void SceneObject::setMaterial(Material* newMaterial)
{
	material = newMaterial;
}
