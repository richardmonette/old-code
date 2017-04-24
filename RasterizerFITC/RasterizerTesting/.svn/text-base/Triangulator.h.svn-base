#pragma once

#include "RasterizerTesting.h"

class Triangulator
{
public:

	static bool Process(Vertex * verticesIn, int n, Vertex * verticesOut, int &nVerticesOut);
	static float Area(Vertex * contour, int n);
	static bool InsideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Px, float Py);

private:

	static bool Snip(Vertex * contour, int u, int v, int w, int n, int *V);

};
