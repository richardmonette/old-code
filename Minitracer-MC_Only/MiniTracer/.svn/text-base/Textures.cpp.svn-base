#include "Textures.h"

void StringExplode(string str, string separator, vector<string>* results)
{
	int found;
	found = str.find_first_of(separator);
	while(found != string::npos){
		if(found > 0){
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0){
		results->push_back(str);
	}
}

Textures::Textures(const string texturesString)
{
	if ( texturesString.compare("n/a") != 0 )
	{
		vector<string> result;
		StringExplode(texturesString, ",", &result);

		vector<string>::iterator it;
		for ( it=result.begin() ; it < result.end(); it++ )
		{
			LoadTexture((*it));
		}
	}
}

Textures::~Textures()
{
}

Vector3f Textures::GetSample(const int index, const float u, const float v)
{
	const float onetwofiftyfifth = 1.0f / 255.0f;

	const unsigned int x = u * (float)(textures[index]->GetWidth()-1);
	const unsigned int y = v * (float)(textures[index]->GetHeight()-1);

	sf::Color color = textures[index]->GetPixel(x, y);

	const float r = (float)color.r * onetwofiftyfifth;
	const float g = (float)color.g * onetwofiftyfifth;
	const float b = (float)color.b * onetwofiftyfifth;

	return Vector3f(r, g, b);
}

void Textures::LoadTexture(const string filename)
{
	sf::Image* image = new sf::Image();
	if (image->LoadFromFile(filename))
	{
		textures.push_back(image);
	}
	else
	{
		cout << "texture error on: " << filename;
	}
}
