#pragma once

#include "RasterizerTesting.h"
#include "Math3D.h"
#include "Frustum.h"
#include "Camera.h"
#include "Texture.h"
#include "Q3/Q3Types.h"

#include <SFML/Graphics.hpp>

class Renderer
{
	public:

		Renderer();
		~Renderer();

		void SetCamera(Camera *c);

		void DrawElements(const int numIndices, const int *indices, const tBSPVertex *v);

		int CreateTexture(const char *filename);
		int CopyLightmap(tBSPLightmap * lightmap);
		sf::Uint8 * GetScreen();

		void ClearFrame();
		void UpdateCameraRotation(int x, int y);

		inline void SetDiffuseTexture(const int t)
		{
			textureID = t;
		}

		inline void SetLightmapTexture(const int l)
		{
			lightmapID = l;
		}

	private:
		
		void DrawTriangle(const Vertex *vertA, const Vertex *vertB, const Vertex *vertC);
		void DrawSegements(int y1, int y2);

		Camera *camera;

		int textureID;
		int lightmapID;

		int nTextures;
		int nLightmaps;

		sf::Uint8 *screen;
		float *depth;

		Mat4x4 worldMatrix;
		Mat4x4 cameraMatrix;
		Mat4x4 projectionMatrix;
		Mat4x4 screenMatrix;

		float dizdx, dizdy;
		float diffuse_duizdx, diffuse_dvizdx, diffuse_duizdy, diffuse_dvizdy;
		float lightmap_duizdx, lightmap_dvizdx, lightmap_duizdy, lightmap_dvizdy;
		float xa, xb, iza;
		float diffuse_uiza, diffuse_viza;
		float lightmap_uiza, lightmap_viza;
		float dxdya, dxdyb, dizdya;
		float diffuse_duizdya, diffuse_dvizdya;
		float lightmap_duizdya, lightmap_dvizdya;

		int * textures[10];
		int * lightmaps[100];
};
