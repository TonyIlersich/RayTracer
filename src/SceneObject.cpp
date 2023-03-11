#include "SceneObject.h"
#include "utils.h"
using namespace std;
using namespace glm;

// #define DEBUG_INTERSECT

SceneObject::SceneObject():
	trans(mat4(1)), invtrans(mat4(1)), surface(nullptr)
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
#ifdef DEBUG_INTERSECT
	cerr << "local ray: " << localRay << '\n';
#endif
	if (surface != nullptr && surface->intersect(localRay, tInterval, hit))
	{
#ifdef DEBUG_INTERSECT
		cerr << " hit self at " << hit.point << " (local), ";
#endif
		hit.sceneObject = this;
		hit.applyTransform(invtrans);
#ifdef DEBUG_INTERSECT
		cerr << hit.point << " (parent)\n";
#endif
		return true;
	}
	bool result = false;
	for (auto& c : children)
	{
		result |= c->intersect(localRay, tInterval, hit);
	}
	if (result)
	{
#ifdef DEBUG_INTERSECT
		cerr << " hit child at " << hit.point << " (local), ";
#endif
		hit.applyTransform(invtrans);
#ifdef DEBUG_INTERSECT
		cerr << hit.point << " (parent)\n";
#endif
		return true;
	}
#ifdef DEBUG_INTERSECT
	cerr << " no hit\n";
#endif
	return false;
}

const mat4& SceneObject::getTrans() const
{
	return trans;
}

const mat4& SceneObject::getInvtrans() const
{
	return invtrans;
}

const std::vector<SceneObject*>& SceneObject::getChildren() const
{
	return children;
}

const Surface* SceneObject::getSurface() const
{
	return surface;
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

void SceneObject::setSurface(const Surface* newSurface)
{
	surface = newSurface;
}

void SceneObject::setMaterial(const Material* newMaterial)
{
	material = newMaterial;
}
