#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "includes.h"

constexpr float tau = 6.283185307179586476925286766559f;
constexpr float goldenRatio = 1.6180339887498948482045868343656f;

constexpr float refractiveIndexAir = 1.0003f;

constexpr glm::vec3 rightward ( 1, 0, 0);
constexpr glm::vec3 leftward  (-1, 0, 0);
constexpr glm::vec3 upward    ( 0, 1, 0);
constexpr glm::vec3 downward  ( 0,-1, 0);
constexpr glm::vec3 forward   ( 0, 0, 1);
constexpr glm::vec3 backward  ( 0, 0,-1);

constexpr glm::mat4 identityTrans(1);

#endif