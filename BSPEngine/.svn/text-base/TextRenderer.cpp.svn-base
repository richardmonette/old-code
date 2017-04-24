#include <windows.h>
#include <stdio.h>

#include "main.h"

#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::BuildFont(HDC hDC)
{
	HFONT	font;
	HFONT	oldfont;

	base = glGenLists(96);

	font = CreateFont(	-MulDiv(48, GetDeviceCaps(hDC, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, "Arial");

	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}

void TextRenderer::KillFont()
{
	glDeleteLists(base, 96);
}

void TextRenderer::RenderText(const char *fmt)
{
	char		text[256];
	va_list		ap;

	if (fmt == NULL)
		return;

	/*va_start(ap, fmt);
	    vsprintf(text, fmt, ap);
	va_end(ap);*/

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
