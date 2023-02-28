#ifndef RANDOM_H
#define RANDOM_H

#include "includes.h"

class Random
{
public:
	Random();

	float getValue();
	float getValue(float high);
	float getValue(float low, float high);
	
	int getValue(int high);
	int getValue(int low, int high);

	float getAngle();

	glm::vec2 getUnitVec2();
	glm::vec2 getInUnitCircle();

	glm::vec3 getUnitVec3();
	glm::vec3 getInUnitSphere();

private:
	std::mt19937 mt;
	std::uniform_real_distribution<float> uniform;
};

#endif
