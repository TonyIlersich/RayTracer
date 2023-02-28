#ifndef SHAPE_H
#define SHAPE_H

#include "includes.h"
#include "Hittable.h"

class Volume : public Hittable
{
public:
	virtual ~Volume() = default;
};

#endif