#include "TextureManager.h"

TextureManager::TextureManager(void)
{
}

TextureManager::~TextureManager(void)
{
	map<string, LPDIRECT3DTEXTURE9>::iterator it;

	for ( it = m_Textures.begin(); it != m_Textures.end(); it++ )
	{
		(*it).second->Release();
	}
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(const char *filename)
{
	if (m_Textures.find(filename) == m_Textures.end())
	{
		return NULL;
	}

	return m_Textures[filename];
}
