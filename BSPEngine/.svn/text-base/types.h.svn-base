#pragma once

#include "Vector3.h"
#include "Vector2.h"

struct tModel
{
	// We are going to jazz this out and just store the min max for the box bboxf_t bbox;         // Bounding box (order : 3 min + 3 max)
	float mins[3];
	float maxs[3];

	int firstface;        // First face for model
	int numfaces;         // Number of faces for model
	int firstbrush;       // First brush for model
	int numbrushes;       // Number of brushes for model
};

// This is our BSP vertex structure
struct tBSPVertex
{
    Vector3 vPosition;			// (x, y, z) position. 
    Vector2 vTextureCoord;		// (u, v) texture coordinate
    Vector2 vLightmapCoord;	    // (u, v) lightmap coordinate
    Vector3 vNormal;			// (x, y, z) normal vector
    unsigned char color[4];		// RGBA color for the vertex 
};

