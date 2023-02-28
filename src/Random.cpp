#include "Random.h"
#include "constants.h"
using namespace std;
using namespace glm;

Random::Random(): mt(), uniform(0.f, 1.f) {}

float Random::getValue()
{
	return uniform(mt);
}

float Random::getValue(float high)
{
	return uniform(mt) * high;
}

float Random::getValue(float low, float high)
{
	return uniform(mt) * (high - low) + low;
}

int Random::getValue(int high)
{
	return uniform_int_distribution<int>(0, high - 1)(mt);
}

int Random::getValue(int low, int high)
{
	return uniform_int_distribution<int>(low, high - 1)(mt);
}

float Random::getAngle()
{
	return getValue(tau);
}

vec2 Random::getUnitVec2()
{
	const float angle = getAngle();
	return vec2(cos(angle), sin(angle));
}

vec2 Random::getInUnitCircle()
{
	return getUnitVec2() * sqrt(getValue());
}

vec3 Random::getUnitVec3()
{
	const float theta = getValue(-tau / 4.f, tau / 4.f);
	const float phi = getAngle();
	return vec3(
		cos(theta) * cos(phi),
		cos(theta) * sin(phi),
		sin(theta));
}

vec3 Random::getInUnitSphere()
{
	return getUnitVec3() * cbrt(getValue());
}
