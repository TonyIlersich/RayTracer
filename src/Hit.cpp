#include "Hit.h"
using namespace glm;

Hit::Hit():
	t(INFINITY), point(0), normal(0), u(0), v(0), sceneObject(nullptr)
{}

Hit::Hit(float t, const glm::vec3& point, const glm::vec3& normal, const vec3& u, const vec3& v, const vec2& uv):
	t(t), point(point), normal(normal), u(u), v(v), uv(uv), sceneObject(nullptr)
{}

Hit::operator bool() const
{
	return !isinf(t);
}

void Hit::applyTransform(const glm::mat4& delta)
{
	// TODO: why does this work
	point = vec3(inverse(delta) * vec4(point, 1.f));
	const mat3 inv = inverse(transpose(mat3(delta)));
	normal = inv * normal;
	u = inv * u;
	v = inv * v;
}
