#pragma once

#include "Vector2.h"

#include <vector>

using namespace std;

struct RenderBucket
{
	RenderBucket(int mnx, int mny, int mxx, int mxy)
	{
		min_x = mnx;
		min_y = mny;
		max_x = mxx;
		max_y = mxy;

		center.setX(min_x);//(max_x - min_x) / 2.0f);
		center.setY(min_y);//(max_y - min_y) / 2.0f);
	}

	int min_x;
	int min_y;
	int max_x;
	int max_y;

	Vector2 center;
};

class WorkGenerator
{
public:
	WorkGenerator(int w_, int h_, int bw, int bh);
	~WorkGenerator();

	bool GetNextBucket(int &min_x, int &min_y, int &max_x, int &max_y);
	void Reset();

private:

	int width;
	int height;

	int bucket_width;
	int bucket_height;

	int current_bucket;

	vector<RenderBucket*> buckets;
};

