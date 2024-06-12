#include "Ray.h"
#include "utils.h"
using namespace std;
using namespace glm;

Ray::Ray(const vec3& origin, const vec3& direction, float refractiveIndex):
	origin(origin), direction(direction), refractiveIndex(refractiveIndex)
{}

vec3 Ray::operator()(float t) const
{
	return origin + t * direction;
}

void Ray::applyTransform(const glm::mat4& delta)
{
	origin = vec3(delta * vec4(origin, 1.f));
	direction = vec3(delta * vec4(direction, 0.f));
}

Ray operator*(const glm::mat4& trans, const Ray& ray)
{
	return Ray(vec3(trans * vec4(ray.origin, 1.f)), vec3(trans * vec4(ray.direction, 0.f)));
}

ostream& operator<<(ostream& out, const Ray& ray)
{
	out << ray.origin << " =" << ray.direction << "=>";
	return out;
}
