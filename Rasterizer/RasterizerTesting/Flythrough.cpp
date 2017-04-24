#include "Flythrough.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "stdio.h"

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

// Waypoints read from file to animate camera

Flythrough::Flythrough(void)
{
	step = 0;

	string line;
	ifstream path;
	path.open("flypathin.path");

	if (path.is_open())
	{
		while (getline(path, line))
		{
			vector<string> results;
			StringExplode(line, " ", &results);

			float x = (float)atof(results[0].c_str());
			float y = (float)atof(results[1].c_str());
			float z = (float)atof(results[2].c_str());

			points.push_back(Vec3(x, y, z));

			float lx = (float)atof(results[3].c_str());
			float ly = (float)atof(results[4].c_str());
			float lz = (float)atof(results[5].c_str());

			looks.push_back(Vec3(lx, ly, lz));
		}

		path.close();
	}
}

Flythrough::~Flythrough(void)
{
}

// Referring to the following in order to develop the piece-wise linear interpolation of way points
// <cite> Jerry Jackson, Linear Interpolation with Ruby http://jcj.net/?p=18 </cite>

void Flythrough::GetNextCameraPostion(Vec3 &p, Vec3 &l)
{
	int stepmin = floor(step);
	int stepmax = ceil(step);

	p = points[stepmin] + (((points[stepmax] - points[stepmin]) * (step - stepmin)) / (stepmax - stepmin));
	l = looks[stepmin] + (((looks[stepmax] - looks[stepmin]) * (step - stepmin)) / (stepmax - stepmin));

	step += 0.05;
	if (step > points.size()-1) step = 0;
}
