#include "Eye.h"
#include "Interval.h"
using namespace std;
using namespace glm;

Eye::Eye(
	const glm::vec3& origin,
	const glm::vec3& forward,
	const glm::vec3& up,
	const glm::vec2& fov
):
	origin(origin),
	forward(normalize(forward)),
	up(normalize(up)),
	fov(fov),
	right(cross(this->forward, this->up)),
	toRightEdge(tan(fov.x * 0.5f) * right),
	toTopEdge(tan(fov.y * 0.5f) * this->up)
{}

Ray Eye::getRay(float x, float y) const
{
	return Ray(origin, forward + x * toRightEdge + y * toTopEdge);
}
