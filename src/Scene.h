#ifndef SCENE_H
#define SCENE_H

#include "includes.h"
#include "Interval.h"
#include "Random.h"
#include "Ray.h"
#include "SceneObject.h"

class Scene
{
public:
	bool intersect(const Ray& ray, Interval<float>& tInterval, Hit& hit) const;

	glm::vec3 getRayColor(const Ray& ray) const;

	void serializeForShader(
		glm::mat4* worldToLocal, GLuint* surfaceType,
		GLuint* surfaceMaterialId, glm::vec4* materialEmission,
		glm::vec4* materialAlbedo, GLuint& surfaceCount,
		std::unordered_map<const Material*, GLuint>& materialIdTable) const;

	SceneObject root;

private:
	glm::vec3 getRayColorRec(const Ray& ray, int maxDepth, const glm::vec3& significance) const;

	void serializeForShaderRec(
		glm::mat4* worldToLocal, GLuint* surfaceType,
		GLuint* surfaceMaterialId, glm::vec4* materialEmission,
		glm::vec4* materialAlbedo, GLuint& surfaceCount,
		std::unordered_map<const Material*, GLuint>& materialIdTable,
		const glm::mat4& currentWorldToLocal, const SceneObject* sceneObject) const;

	mutable Random rayRandom;
};

#endif