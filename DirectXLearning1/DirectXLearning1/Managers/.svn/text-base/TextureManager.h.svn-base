#pragma once

#include "../Global.h"

#include <d3d9.h>
#include <d3dx9.h>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	LPDIRECT3DTEXTURE9 GetTexture(const char *filename);

private:

	map <string, LPDIRECT3DTEXTURE9> m_Textures;

};
