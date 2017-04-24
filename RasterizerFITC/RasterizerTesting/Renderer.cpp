#include "Renderer.h"

#include "FastCast.h"

#include <emmintrin.h>
#include <xmmintrin.h>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>

int NO_DRAW_ID = -1;

void Renderer::SetCamera(Camera *c)
{
	camera = c;
}

Renderer::Renderer()
{	
	screen = new sf::Uint8[(resX*resY)*4];
	depth = new float[resX*resY];

	nTextures = 0;
	nLightmaps = 0;

	screenMatrix = CreateViewportScaleMatrix(resX, resY, resX/2, -resY/2);
	SetMatrixIdentity(&worldMatrix);

	dizdx = diffuse_duizdx = diffuse_dvizdx = dizdy = diffuse_duizdy = diffuse_dvizdy = 0;
	xa = xb = iza = diffuse_uiza = diffuse_viza = 0;
	dxdya = dxdyb = dizdya = diffuse_duizdya = diffuse_dvizdya = 0;

	return;
}

Renderer::~Renderer()
{	
	delete screen;
	delete depth;
}

void Renderer::ClearFrame()
{
	memset(depth, 0, resX * resY * sizeof(float));
	memset(screen, 0, resX * resY * 4);

	cameraMatrix = *camera->GetViewMatrix();
	projectionMatrix = *camera->GetProjectionMatrix();
}

inline void VECTOR4D_Build(Vec4 *init, Vec4 *term, Vec4 *result)
{
	// build a 4d vector
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
	result->w = 1.0;
}

#define CLIP_CODE_GZ 0x0001 // z > z_max
#define CLIP_CODE_LZ 0x0002 // z < z_min
#define CLIP_CODE_IZ 0x0004 // z_min < z < z_max

void Renderer::DrawElements(const int numIndices, const int *indices, const tBSPVertex *vertices)
{	
	if (textureID == NO_DRAW_ID)
		return;

	int zClipCodes[3];
	int v0, v1, v2;

	Vertex verts[3];
	Vertex temp_poly[3];
	Vec4 v;

	const float NEARCLIP = 5;
	const float FARCLIP = 1000;

	for (int p = 0; p < numIndices; p += 3) 
	{		
		int numVertsIn = 0;

		// vert 0

			// OBJECT SPACE

			verts[0].position.x = vertices[indices[p+0]].vPosition.x;
			verts[0].position.y = vertices[indices[p+0]].vPosition.y;
			verts[0].position.z = vertices[indices[p+0]].vPosition.z;
			verts[0].position.w = 1.0f;

			verts[0].diffuse_uv.u = vertices[indices[p+0]].vTextureCoord.x;
			verts[0].diffuse_uv.v = vertices[indices[p+0]].vTextureCoord.y;

			verts[0].lightmap_uv.u = vertices[indices[p+0]].vLightmapCoord.x;
			verts[0].lightmap_uv.v = vertices[indices[p+0]].vLightmapCoord.y;

			// WORLD SPACE

			//verts[0].position = verts[0].position * worldMatrix;

			// CAMERA SPACE

			Vec4ByMat4x4(&verts[0].position, &verts[0].position, &cameraMatrix);

			// CLIP

			if (verts[0].position.z > FARCLIP)
			{
				zClipCodes[0] = CLIP_CODE_GZ;
			}
			else if (verts[0].position.z < NEARCLIP)
			{
				zClipCodes[0] = CLIP_CODE_LZ;
			}
			else
			{
				zClipCodes[0] = CLIP_CODE_IZ;
				numVertsIn++;
			}

		// vert 1

			// OBJECT SPACE

			verts[1].position.x = vertices[indices[p+1]].vPosition.x;
			verts[1].position.y = vertices[indices[p+1]].vPosition.y;
			verts[1].position.z = vertices[indices[p+1]].vPosition.z;
			verts[1].position.w = 1.0f;

			verts[1].diffuse_uv.u = vertices[indices[p+1]].vTextureCoord.x;
			verts[1].diffuse_uv.v = vertices[indices[p+1]].vTextureCoord.y;

			verts[1].lightmap_uv.u = vertices[indices[p+1]].vLightmapCoord.x;
			verts[1].lightmap_uv.v = vertices[indices[p+1]].vLightmapCoord.y;

			// WORLD SPACE

			//verts[1].position = verts[1].position * worldMatrix;

			// CAMERA SPACE

			Vec4ByMat4x4(&verts[1].position, &verts[1].position, &cameraMatrix);

			// CLIP

			if (verts[1].position.z > FARCLIP)
			{
				zClipCodes[1] = CLIP_CODE_GZ;
			}
			else if (verts[1].position.z < NEARCLIP)
			{
				zClipCodes[1] = CLIP_CODE_LZ;
			}
			else
			{
				zClipCodes[1] = CLIP_CODE_IZ;
				numVertsIn++;
			}

		// vert 2

			// OBJECT SPACE

			verts[2].position.x = vertices[indices[p+2]].vPosition.x;
			verts[2].position.y = vertices[indices[p+2]].vPosition.y;
			verts[2].position.z = vertices[indices[p+2]].vPosition.z;
			verts[2].position.w = 1.0f;

			verts[2].diffuse_uv.u = vertices[indices[p+2]].vTextureCoord.x;
			verts[2].diffuse_uv.v = vertices[indices[p+2]].vTextureCoord.y;

			verts[2].lightmap_uv.u = vertices[indices[p+2]].vLightmapCoord.x;
			verts[2].lightmap_uv.v = vertices[indices[p+2]].vLightmapCoord.y;

			// WORLD SPACE

			//verts[2].position = verts[2].position * worldMatrix;

			// CAMERA SPACE

			Vec4ByMat4x4(&verts[2].position, &verts[2].position, &cameraMatrix);

			// CLIP

			if (verts[2].position.z > FARCLIP)
			{
				zClipCodes[2] = CLIP_CODE_GZ;
			}
			else if (verts[2].position.z < NEARCLIP)
			{
				zClipCodes[2] = CLIP_CODE_LZ;
			}
			else
			{
				zClipCodes[2] = CLIP_CODE_IZ;
				numVertsIn++;
			}
		

		/* BACKFACE CULL

		{
			Vec3 side1(verts[0].position.x - verts[1].position.x,
				   verts[0].position.y - verts[1].position.y,
				   verts[0].position.z - verts[1].position.z);

			Vec3 side2(verts[0].position.x - verts[2].position.x,
				   verts[0].position.y - verts[2].position.y,
				   verts[0].position.z - verts[2].position.z);

			Vec3 view(0,0,1);
			
			// compute cross product
			Vec3 n = cross(side1, side2);

			if (dot(n, view) < 0)
			{
				continue;
			}
		}*/

		if ( verts[0].position.x < -verts[0].position.z && verts[1].position.x < -verts[1].position.z && verts[2].position.x < -verts[2].position.z || 
				verts[0].position.x > verts[0].position.z && verts[1].position.x > verts[1].position.z && verts[2].position.x > verts[2].position.z )
		{
			continue;
		}

		if ( verts[0].position.y < -verts[0].position.z && verts[1].position.y < -verts[1].position.z && verts[2].position.y < -verts[2].position.z || 
				verts[0].position.y > verts[0].position.z && verts[1].position.y > verts[1].position.z && verts[2].position.y > verts[2].position.z )
		{
			continue;
		}

		// CHECK TRIVIAL REJECTION NEAR FAR

		if ( ((zClipCodes[0] == CLIP_CODE_GZ) && (zClipCodes[1] == CLIP_CODE_GZ) && (zClipCodes[2] == CLIP_CODE_GZ) ) ||
			((zClipCodes[0] == CLIP_CODE_LZ) && (zClipCodes[1] == CLIP_CODE_LZ) && (zClipCodes[2] == CLIP_CODE_LZ) ) )
		{
			continue;
		}

		// IS ANYTHING CLIPPING THE NEAR PLANE

		if ( ( (zClipCodes[0] | zClipCodes[1] | zClipCodes[2]) & CLIP_CODE_LZ) )
		{
			if (numVertsIn == 1) // NICE CASE NO NEW VERTICES
			{
				if ( zClipCodes[0] == CLIP_CODE_IZ)
				{ 
					v0 = 0; v1 = 1; v2 = 2;
				}
				else if (zClipCodes[1] == CLIP_CODE_IZ)
				{ 
					v0 = 1; v1 = 2; v2 = 0;
				}
				else
				{ 
					v0 = 2; v1 = 0; v2 = 1;
				}

				// clip edge v0->v1
				
				VECTOR4D_Build(&verts[v0].position, &verts[v1].position, &v); 
				
				// the intersection occurs when z = near z, so t =
				const float t1 = ( (NEARCLIP - verts[v0].position.z) / v.z );
				
				// now plug t back in and find x,y intersection with the plane
				float xi = verts[v0].position.x + v.x * t1;
				float yi = verts[v0].position.y + v.y * t1;
				
				// now overwrite vertex with new vertex
				verts[v1].position.x = xi;
				verts[v1].position.y = yi;
				verts[v1].position.z = NEARCLIP;
				
				// clip edge v0->v2
				
				VECTOR4D_Build(&verts[v0].position, &verts[v2].position, &v);

				// the intersection occurs when z = near z, so t =
				const float t2 = ( (NEARCLIP - verts[v0].position.z) / v.z );

				// now plug t back in and find x,y intersection with the plane
				xi = verts[v0].position.x + v.x * t2;
				yi = verts[v0].position.y + v.y * t2;

				// now overwrite vertex with new vertex
				verts[v2].position.x = xi;
				verts[v2].position.y = yi;
				verts[v2].position.z = NEARCLIP;
			

				// CLIP DIFFUSE TEXTURE COORDS

				float ui = verts[v0].diffuse_uv.u + (verts[v1].diffuse_uv.u - verts[v0].diffuse_uv.u) * t1;
				float vi = verts[v0].diffuse_uv.v + (verts[v1].diffuse_uv.v - verts[v0].diffuse_uv.v) * t1;
				
				verts[v1].diffuse_uv.u = ui;
				verts[v1].diffuse_uv.v = vi;
				
				ui = verts[v0].diffuse_uv.u + (verts[v2].diffuse_uv.u - verts[v0].diffuse_uv.u) * t2;
				vi = verts[v0].diffuse_uv.v + (verts[v2].diffuse_uv.v - verts[v0].diffuse_uv.v) * t2;
				
				verts[v2].diffuse_uv.u = ui;
				verts[v2].diffuse_uv.v = vi;

				// CLIP LIGHTMAP TEXTURE COORDS

				ui = verts[v0].lightmap_uv.u + (verts[v1].lightmap_uv.u - verts[v0].lightmap_uv.u) * t1;
				vi = verts[v0].lightmap_uv.v + (verts[v1].lightmap_uv.v - verts[v0].lightmap_uv.v) * t1;
				
				verts[v1].lightmap_uv.u = ui;
				verts[v1].lightmap_uv.v = vi;
				
				ui = verts[v0].lightmap_uv.u + (verts[v2].lightmap_uv.u - verts[v0].lightmap_uv.u) * t2;
				vi = verts[v0].lightmap_uv.v + (verts[v2].lightmap_uv.v - verts[v0].lightmap_uv.v) * t2;
				
				verts[v2].lightmap_uv.u = ui;
				verts[v2].lightmap_uv.v = vi;

				// DRAW

				Vec4ByMat4x4(&verts[v0].position, &verts[v0].position, &projectionMatrix);
				Vec4ByMat4x4(&verts[v1].position, &verts[v1].position, &projectionMatrix);
				Vec4ByMat4x4(&verts[v2].position, &verts[v2].position, &projectionMatrix);

				verts[v0].invz = 1.0f / verts[v0].position.z;
				verts[v1].invz = 1.0f / verts[v1].position.z;
				verts[v2].invz = 1.0f / verts[v2].position.z;

				const float inverseW0 = 1.0f / verts[v0].position.w;
				const float inverseW1 = 1.0f / verts[v1].position.w;
				const float inverseW2 = 1.0f / verts[v2].position.w;

				Vec4ByScalar(&verts[v0].position, inverseW0);
				Vec4ByScalar(&verts[v1].position, inverseW1);
				Vec4ByScalar(&verts[v2].position, inverseW2);

				Vec4ByMat4x4(&verts[v0].position, &verts[v0].position, &screenMatrix);
				Vec4ByMat4x4(&verts[v1].position, &verts[v1].position, &screenMatrix);
				Vec4ByMat4x4(&verts[v2].position, &verts[v2].position, &screenMatrix);

				DrawTriangle(&verts[v0], &verts[v1], &verts[v2]);
			}
			else if (numVertsIn == 2)
			{
				temp_poly[0] = verts[0];
				temp_poly[1] = verts[1];
				temp_poly[2] = verts[2];

				// find interior vertex

				if ( zClipCodes[0] == CLIP_CODE_LZ)
				{ 
					v0 = 0; v1 = 1; v2 = 2;
				}
				else if (zClipCodes[1] == CLIP_CODE_LZ)
				{ 
					v0 = 1; v1 = 2; v2 = 0; 
				}
				else
				{ 
					v0 = 2; v1 = 0; v2 = 1; 
				}

				// clip edge v0->v1
				VECTOR4D_Build(&verts[v0].position, &verts[v1].position, &v);
				
				// the intersection occurs when z = near z, so t =
				const float t1 = ( (NEARCLIP - verts[v0].position.z) / v.z );
				
				// now plug t back in and find x,y intersection with the plane
				const float x01i = verts[v0].position.x + v.x * t1;
				const float y01i = verts[v0].position.y + v.y * t1;
				
				// clip edge v0->v2
				VECTOR4D_Build(&verts[v0].position, &verts[v2].position, &v);
				
				// the intersection occurs when z = near z, so t =
				const float t2 = ( (NEARCLIP - verts[v0].position.z) / v.z );
				
				// now plug t back in and find x,y intersection with the plane
				const float x02i = verts[v0].position.x + v.x * t2;
				const float y02i = verts[v0].position.y + v.y * t2;
				
				verts[v0].position.x = x01i;
				verts[v0].position.y = y01i;
				verts[v0].position.z = NEARCLIP;

				temp_poly[v1].position.x = x01i;
				temp_poly[v1].position.y = y01i;
				temp_poly[v1].position.z = NEARCLIP;
				temp_poly[v0].position.x = x02i;
				temp_poly[v0].position.y = y02i;
				temp_poly[v0].position.z = NEARCLIP;

				// fix UVS

				float u01i = verts[v0].diffuse_uv.u + (verts[v1].diffuse_uv.u - verts[v0].diffuse_uv.u) * t1;
				float v01i = verts[v0].diffuse_uv.v + (verts[v1].diffuse_uv.v - verts[v0].diffuse_uv.v) * t1;
				
				// compute poly 2 new texture coordinates from split
				float u02i = verts[v0].diffuse_uv.u + (verts[v2].diffuse_uv.u - verts[v0].diffuse_uv.u) * t2;
				float v02i = verts[v0].diffuse_uv.v + (verts[v2].diffuse_uv.v - verts[v0].diffuse_uv.v) * t2;
				
				// write them all at the same time
				
				// poly 1
				verts[v0].diffuse_uv.u = u01i;
				verts[v0].diffuse_uv.v = v01i;
				
				// poly 2
				temp_poly[v0].diffuse_uv.u = u02i;
				temp_poly[v0].diffuse_uv.v = v02i;
				temp_poly[v1].diffuse_uv.u = u01i;
				temp_poly[v1].diffuse_uv.v = v01i;

				// LIGHTMAPS

				u01i = verts[v0].lightmap_uv.u + (verts[v1].lightmap_uv.u - verts[v0].lightmap_uv.u) * t1;
				v01i = verts[v0].lightmap_uv.v + (verts[v1].lightmap_uv.v - verts[v0].lightmap_uv.v) * t1;
				
				// compute poly 2 new texture coordinates from split
				u02i = verts[v0].lightmap_uv.u + (verts[v2].lightmap_uv.u - verts[v0].lightmap_uv.u) * t2;
				v02i = verts[v0].lightmap_uv.v + (verts[v2].lightmap_uv.v - verts[v0].lightmap_uv.v) * t2;
				
				// write them all at the same time
				
				// poly 1

				verts[v0].lightmap_uv.u = u01i;
				verts[v0].lightmap_uv.v = v01i;
				
				// poly 2

				temp_poly[v0].lightmap_uv.u = u02i;
				temp_poly[v0].lightmap_uv.v = v02i;
				temp_poly[v1].lightmap_uv.u = u01i;
				temp_poly[v1].lightmap_uv.v = v01i;

				// DRAW ONE

				Vec4ByMat4x4(&verts[v0].position, &verts[v0].position, &projectionMatrix);
				Vec4ByMat4x4(&verts[v1].position, &verts[v1].position, &projectionMatrix);
				Vec4ByMat4x4(&verts[v2].position, &verts[v2].position, &projectionMatrix);

				verts[v0].invz = 1.0f / verts[v0].position.z;
				verts[v1].invz = 1.0f / verts[v1].position.z;
				verts[v2].invz = 1.0f / verts[v2].position.z;

				float inverseW0 = 1.0f / verts[v0].position.w;
				float inverseW1 = 1.0f / verts[v1].position.w;
				float inverseW2 = 1.0f / verts[v2].position.w;

				Vec4ByScalar(&verts[v0].position, inverseW0);
				Vec4ByScalar(&verts[v1].position, inverseW0);
				Vec4ByScalar(&verts[v2].position, inverseW0);

				Vec4ByMat4x4(&verts[v0].position, &verts[v0].position, &screenMatrix);
				Vec4ByMat4x4(&verts[v1].position, &verts[v1].position, &screenMatrix);
				Vec4ByMat4x4(&verts[v2].position, &verts[v2].position, &screenMatrix);

				DrawTriangle(&verts[v0], &verts[v1], &verts[v2]);

				// DRAW TWO

				Vec4ByMat4x4(&temp_poly[v0].position, &temp_poly[v0].position, &projectionMatrix);
				Vec4ByMat4x4(&temp_poly[v1].position, &temp_poly[v1].position, &projectionMatrix);
				Vec4ByMat4x4(&temp_poly[v2].position, &temp_poly[v2].position, &projectionMatrix);

				temp_poly[v0].invz = 1.0f / temp_poly[v0].position.z;
				temp_poly[v1].invz = 1.0f / temp_poly[v1].position.z;
				temp_poly[v2].invz = 1.0f / temp_poly[v2].position.z;

				inverseW0 = 1.0f / temp_poly[v0].position.w;
				inverseW1 = 1.0f / temp_poly[v1].position.w;
				inverseW2 = 1.0f / temp_poly[v2].position.w;

				Vec4ByScalar(&temp_poly[v0].position, inverseW0);
				Vec4ByScalar(&temp_poly[v1].position, inverseW1);
				Vec4ByScalar(&temp_poly[v2].position, inverseW2);

				Vec4ByMat4x4(&temp_poly[v0].position, &temp_poly[v0].position, &screenMatrix);
				Vec4ByMat4x4(&temp_poly[v1].position, &temp_poly[v1].position, &screenMatrix);
				Vec4ByMat4x4(&temp_poly[v2].position, &temp_poly[v2].position, &screenMatrix);

				DrawTriangle(&temp_poly[v0], &temp_poly[v1], &temp_poly[v2]);
			}
		}
		else // NO INTERSECTION
		{
			Vec4ByMat4x4(&verts[0].position, &verts[0].position, &projectionMatrix);
			Vec4ByMat4x4(&verts[1].position, &verts[1].position, &projectionMatrix);
			Vec4ByMat4x4(&verts[2].position, &verts[2].position, &projectionMatrix);

			verts[0].invz = 1.0f / verts[0].position.z;
			verts[1].invz = 1.0f / verts[1].position.z;
			verts[2].invz = 1.0f / verts[2].position.z;

			const float inverseW0 = 1.0f / verts[0].position.w;
			const float inverseW1 = 1.0f / verts[1].position.w;
			const float inverseW2 = 1.0f / verts[2].position.w;

			Vec4ByScalar(&verts[0].position, inverseW0);
			Vec4ByScalar(&verts[1].position, inverseW1);
			Vec4ByScalar(&verts[2].position, inverseW2);

			Vec4ByMat4x4(&verts[0].position, &verts[0].position, &screenMatrix);
			Vec4ByMat4x4(&verts[1].position, &verts[1].position, &screenMatrix);
			Vec4ByMat4x4(&verts[2].position, &verts[2].position, &screenMatrix);

			DrawTriangle(&verts[0], &verts[1], &verts[2]);
		}
	}
	
	return;
}

inline int ftoi_sse1(float f)
{
    return _mm_cvtt_ss2si(_mm_load_ss(&f));     // SSE1 instructions for float->int
}

void Renderer::DrawSegements(int y1, int y2)
{	
	const int * texture = textures[textureID];
	const int * lightmap = lightmaps[lightmapID];

	if (y2 > resY)
	{
		y2 = resY;
	}

	while (y1++ < y2)
	{
		int x1 = ftoi_sse1(xa);
		int x2 = ftoi_sse1(xb);

		if (x1 < 0)
		{
			x1 = 0;
		}

		if (x2 > resX)
		{
			x2 = resX;
		}

		// ?
		const float dx = 1.0f - (xa - x1);

		float iz = iza + dx * dizdx;

		float diffuse_uiz = diffuse_uiza + dx * diffuse_duizdx;
		float diffuse_viz = diffuse_viza + dx * diffuse_dvizdx;

		float lightmap_uiz = lightmap_uiza + dx * lightmap_duizdx;
		float lightmap_viz = lightmap_viza + dx * lightmap_dvizdx;

		float *dpth = &depth[y1 * resX + x1];
		sf::Uint8 *pixel = &screen[ (( y1 * resX ) + x1) << 2 ];
		
		while (x1++ < x2)
		{
			//if (y1 > 0)
			{
				if (iz > *dpth)
				{			
					const float z_recip = 1.0f / iz;

					float diffuse_u = diffuse_uiz * z_recip;
					float diffuse_v = diffuse_viz * z_recip;

					float lightmap_u = lightmap_uiz * z_recip;
					float lightmap_v = lightmap_viz * z_recip;

					//

					diffuse_u = (diffuse_u + 16.f);
					diffuse_u -= ftoi_sse1(diffuse_u);

					diffuse_v = (diffuse_v + 16.f);
					diffuse_v -= ftoi_sse1(diffuse_v);

					diffuse_u *= 128.0f;
					diffuse_v *= 128.0f;

					//

					lightmap_u = (lightmap_u + 16.f);
					lightmap_u -= ftoi_sse1(lightmap_u);

					lightmap_v = (lightmap_v + 16.f);
					lightmap_v -= ftoi_sse1(lightmap_v);

					lightmap_u *= 128.0f;
					lightmap_v *= 128.0f;

					const int color = texture[ ( ftoi_sse1(diffuse_v) << 7 ) + ftoi_sse1(diffuse_u) ];
					const int light = lightmap[ ( ftoi_sse1(lightmap_v) << 7 ) + ftoi_sse1(lightmap_u) ];
						
					const int color_r = ((color >> 16) & 0xFF);
					const int color_g = ((color >> 8) & 0xFF);
					const int color_b = (color & 0xFF);

					const int light_r = ((light >> 16) & 0xFF);
					const int light_g = ((light >> 8) & 0xFF);
					const int light_b = (light & 0xFF);

					const int mix_r = (color_r * light_r) >> 8;
					const int mix_g = (color_g * light_g) >> 8;
					const int mix_b = (color_b * light_b) >> 8;
					
					*pixel     = mix_r;
					*(pixel+1) = mix_g;
					*(pixel+2) = mix_b;
					*(pixel+3) = 255;

					*dpth = iz;
				}
			}

			dpth++;
			pixel += 4;

			iz += dizdx;

			diffuse_uiz += diffuse_duizdx;
			diffuse_viz += diffuse_dvizdx;

			lightmap_uiz += lightmap_duizdx;
			lightmap_viz += lightmap_dvizdx;
		}

		xa += dxdya;
		xb += dxdyb;
		iza += dizdya;

		diffuse_uiza += diffuse_duizdya;
		diffuse_viza += diffuse_dvizdya;

		lightmap_uiza += lightmap_duizdya;
		lightmap_viza += lightmap_dvizdya;
	}
}



inline void rswap(float &x, float &y)
{
	float swap_temp;

	swap_temp = x;
	x = y;
	y = swap_temp;
}

void Renderer::DrawTriangle(const Vertex *vertA, const Vertex *vertB, const Vertex *vertC)
{			
	float x1 = vertA->position.x;
	float y1 = vertA->position.y;
	float x2 = vertB->position.x;
	float y2 = vertB->position.y;
	float x3 = vertC->position.x;
	float y3 = vertC->position.y;

	float iz1 = vertA->invz;
	float iz2 = vertB->invz;
	float iz3 = vertC->invz;
	
	float diffuse_uiz1 = vertA->diffuse_uv.u * iz1;
	float diffuse_viz1 = vertA->diffuse_uv.v * iz1;
	float diffuse_uiz2 = vertB->diffuse_uv.u * iz2;
	float diffuse_viz2 = vertB->diffuse_uv.v * iz2;
	float diffuse_uiz3 = vertC->diffuse_uv.u * iz3;
	float diffuse_viz3 = vertC->diffuse_uv.v * iz3;

	float lightmap_uiz1 = vertA->lightmap_uv.u * iz1;
	float lightmap_viz1 = vertA->lightmap_uv.v * iz1;
	float lightmap_uiz2 = vertB->lightmap_uv.u * iz2;
	float lightmap_viz2 = vertB->lightmap_uv.v * iz2;
	float lightmap_uiz3 = vertC->lightmap_uv.u * iz3;
	float lightmap_viz3 = vertC->lightmap_uv.v * iz3;

	if ( y1 > y2 )
	{
		rswap(x1, x2);
		rswap(y1, y2);
		rswap(iz1, iz2);
		rswap(diffuse_uiz1, diffuse_uiz2);
		rswap(diffuse_viz1, diffuse_viz2);
		rswap(lightmap_uiz1, lightmap_uiz2);
		rswap(lightmap_viz1, lightmap_viz2);
	}
	if ( y1 > y3 )
	{
		rswap(x1, x3);
		rswap(y1, y3);
		rswap(iz1, iz3);
		rswap(diffuse_uiz1, diffuse_uiz3);
		rswap(diffuse_viz1, diffuse_viz3);
		rswap(lightmap_uiz1, lightmap_uiz3);
		rswap(lightmap_viz1, lightmap_viz3);
	}
	if ( y2 > y3 )
	{
		rswap(x2, x3);
		rswap(y2, y3);
		rswap(iz2, iz3);
		rswap(diffuse_uiz2, diffuse_uiz3);
		rswap(diffuse_viz2, diffuse_viz3);
		rswap(lightmap_uiz2, lightmap_uiz3);
		rswap(lightmap_viz2, lightmap_viz3);
	}

	int y1i = ftoi_sse1(y1);
	int y2i = ftoi_sse1(y2);
	int y3i = ftoi_sse1(y3);

	if ((y1i == y2i && y1i == y3i) || ((int) x1 == (int) x2 && (int) x1 == (int) x3))
	{
		return;		
	}

	float denom = ((x3 - x1) * (y2 - y1) - (x2 - x1) * (y3 - y1));

	if (!denom)
	{
		return;
	}	

	denom = 1.0f / denom;
	
	dizdx =  ((iz3 - iz1)   * (y2 - y1) - (iz2 - iz1)   * (y3 - y1)) * denom;
	
	diffuse_duizdx = ((diffuse_uiz3 - diffuse_uiz1) * (y2 - y1) - (diffuse_uiz2 - diffuse_uiz1) * (y3 - y1)) * denom;
	diffuse_dvizdx = ((diffuse_viz3 - diffuse_viz1) * (y2 - y1) - (diffuse_viz2 - diffuse_viz1) * (y3 - y1)) * denom;
	
	lightmap_duizdx = ((lightmap_uiz3 - lightmap_uiz1) * (y2 - y1) - (lightmap_uiz2 - lightmap_uiz1) * (y3 - y1)) * denom;
	lightmap_dvizdx = ((lightmap_viz3 - lightmap_viz1) * (y2 - y1) - (lightmap_viz2 - lightmap_viz1) * (y3 - y1)) * denom;
	
	dizdy =  ((iz2 - iz1)   * (x3 - x1) - (iz3 - iz1)   * (x2 - x1)) * denom;
	
	diffuse_duizdy = ((diffuse_uiz2 - diffuse_uiz1) * (x3 - x1) - (diffuse_uiz3 - diffuse_uiz1) * (x2 - x1)) * denom;
	diffuse_dvizdy = ((diffuse_viz2 - diffuse_viz1) * (x3 - x1) - (diffuse_viz3 - diffuse_viz1) * (x2 - x1)) * denom;
	
	lightmap_duizdy = ((lightmap_uiz2 - lightmap_uiz1) * (x3 - x1) - (lightmap_uiz3 - lightmap_uiz1) * (x2 - x1)) * denom;
	lightmap_dvizdy = ((lightmap_viz2 - lightmap_viz1) * (x3 - x1) - (lightmap_viz3 - lightmap_viz1) * (x2 - x1)) * denom;

	float dxdy1 = 0;
	float dxdy2 = 0;
	float dxdy3 = 0;

	if (y2 > y1)
		dxdy1 = (x2 - x1) / (y2 - y1);
	if (y3 > y1)
		dxdy2 = (x3 - x1) / (y3 - y1);
	if (y3 > y2)
		dxdy3 = (x3 - x2) / (y3 - y2);

	int side = dxdy2 > dxdy1;

	if (y1 == y2)
		side = x1 > x2;
	if (y2 == y3)
		side = x3 > x2;

	if (!side)
	{
		if (y1i < 0) y1i = 0;
		if (y2i < 0) y2i = 0;

		dxdya = dxdy2;
		dizdya = dxdy2 * dizdx + dizdy;
		diffuse_duizdya = dxdy2 * diffuse_duizdx + diffuse_duizdy;
		diffuse_dvizdya = dxdy2 * diffuse_dvizdx + diffuse_dvizdy;
		lightmap_duizdya = dxdy2 * lightmap_duizdx + lightmap_duizdy;
		lightmap_dvizdya = dxdy2 * lightmap_dvizdx + lightmap_dvizdy;

		float dy = 1 - (y1 - y1i);
		xa = x1 + dy * dxdya;
		iza = iz1 + dy * dizdya;
		diffuse_uiza = diffuse_uiz1 + dy * diffuse_duizdya;
		diffuse_viza = diffuse_viz1 + dy * diffuse_dvizdya;
		lightmap_uiza = lightmap_uiz1 + dy * lightmap_duizdya;
		lightmap_viza = lightmap_viz1 + dy * lightmap_dvizdya;

		if (y1i < y2i)
		{
			xb = x1 + dy * dxdy1;
			dxdyb = dxdy1;

			DrawSegements(y1i, y2i);
		}

		if (y2i < y3i)
		{
			xb = x2 + (1 - (y2 - y2i)) * dxdy3;
			dxdyb = dxdy3;

			DrawSegements(y2i, y3i);
		}
	}
	else
	{
		if (y1i < 0) y1i = 0;
		if (y2i < 0) y2i = 0;

		dxdyb = dxdy2;
		float dy = 1 - (y1 - y1i);
		xb = x1 + dy * dxdyb;

		if (y1i < y2i)
		{
			dxdya = dxdy1;
			dizdya = dxdy1 * dizdx + dizdy;
			diffuse_duizdya = dxdy1 * diffuse_duizdx + diffuse_duizdy;
			diffuse_dvizdya = dxdy1 * diffuse_dvizdx + diffuse_dvizdy;
			lightmap_duizdya = dxdy1 * lightmap_duizdx + lightmap_duizdy;
			lightmap_dvizdya = dxdy1 * lightmap_dvizdx + lightmap_dvizdy;
			xa = x1 + dy * dxdya;
			iza = iz1 + dy * dizdya;
			diffuse_uiza = diffuse_uiz1 + dy * diffuse_duizdya;
			diffuse_viza = diffuse_viz1 + dy * diffuse_dvizdya;
			lightmap_uiza = lightmap_uiz1 + dy * lightmap_duizdya;
			lightmap_viza = lightmap_viz1 + dy * lightmap_dvizdya;

			DrawSegements(y1i, y2i);
		}

		if (y2i < y3i)
		{
			dxdya = dxdy3;
			dizdya = dxdy3 * dizdx + dizdy;
			diffuse_duizdya = dxdy3 * diffuse_duizdx + diffuse_duizdy;
			diffuse_dvizdya = dxdy3 * diffuse_dvizdx + diffuse_dvizdy;
			lightmap_duizdya = dxdy3 * lightmap_duizdx + lightmap_duizdy;
			lightmap_dvizdya = dxdy3 * lightmap_dvizdx + lightmap_dvizdy;
			dy = 1 - (y2 - y2i);
			xa = x2 + dy * dxdya;
			iza = iz2 + dy * dizdya;
			diffuse_uiza = diffuse_uiz2 + dy * diffuse_duizdya;
			diffuse_viza = diffuse_viz2 + dy * diffuse_dvizdya;
			lightmap_uiza = lightmap_uiz2 + dy * lightmap_duizdya;
			lightmap_viza = lightmap_viz2 + dy * lightmap_dvizdya;
			
			DrawSegements(y2i, y3i);
		}
	}
}	

sf::Uint8 * Renderer::GetScreen()
{
	return screen;
}

int Renderer::CopyLightmap(tBSPLightmap * lightmap)
{
	lightmaps[nLightmaps] = new int[128*128];

	int index = 0;
	unsigned char * pImage = (unsigned char*)lightmap->imageBits;
	for(int i = 0; i < 128*128*3 / 3; i++, pImage += 3) 
	{
		const int out = ( ( (int)pImage[0] << 16 ) | ( (int)pImage[1] << 8 ) | ( (int)pImage[2] ) );

		lightmaps[nLightmaps][index] = out;

		index++;
	}

	nLightmaps++;

	return (nLightmaps - 1);
}

int Renderer::CreateTexture(const char* filename)
{
	//Load up the requested filename into the first available texture slot
	textures[nTextures] = new int[128 * 128];

	sf::Image image;
	image.LoadFromFile(filename);

	int sizeofint = sizeof(int);
	memcpy(textures[nTextures], image.GetPixelsPtr(), 128 * 128 * sizeofint);

	if (strstr(filename, "NULL") != NULL)
	{
		NO_DRAW_ID = nTextures;
	}

	//Increase the number of active textures
	nTextures++;

	//Return the position -1 since thats where the requested texture is actually residing
	return (nTextures - 1);
}


