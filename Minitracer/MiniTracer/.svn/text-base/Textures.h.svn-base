#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Vector3f.hpp"

using namespace std;
using namespace hxa7241_graphics;

class Textures
{
public:
	Textures(const string texturesString);
	~Textures();

	void LoadTexture(const string filename);
	Vector3f GetSample(const int index, const float u, const float v);

	vector<sf::Image*> textures;
};
