#pragma once

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	GLuint	base;

	void RenderText(const char *text);
	
	void BuildFont(HDC hDC);
	void KillFont();

};
