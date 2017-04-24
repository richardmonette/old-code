#pragma once

#include "Main.h"
#include "WaterShader.h"
#include "Vector3.h"

class Water
{
public:
	Water();
	~Water();

	void Render(Vector3 *cameraPosition);
	void CreateDisplayList();

	void CreatePlane();

	WaterShader *waterShader;

	float waterDisplacement;
	float height;
};
