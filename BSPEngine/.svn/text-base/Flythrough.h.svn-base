#pragma once

#include "Vector3.h"
#include <vector>

using namespace std;

class Flythrough
{
public:
	Flythrough();
	~Flythrough();

	void GetNextCameraPostion(Vector3 &p, Vector3 &l);

private:

	vector<Vector3> points;
	vector<Vector3> looks;
	float step;
};

