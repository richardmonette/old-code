#pragma once

#include "Transform.h"

class Shape : ReferenceCounted
{
public:
	Shape(const Transform &o2w, bool ro);

	const Transform ObjectToWorld, WorldToObject;
	const bool reverseOrientation, transformSwapHandedness;

	~Shape();
};
