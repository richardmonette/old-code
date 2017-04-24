
#include "WorkGenerator.h"

#include <algorithm>

Vector2 center;

struct BucketSorter
{
	bool operator()(RenderBucket *b0, RenderBucket *b1)
	{ 
		float d0 = (center - b0->center).length();
		float d1 = (center - b1->center).length();

		return (d0 < d1); 
	}
};

WorkGenerator::WorkGenerator(int w_, int h_, int bw, int bh)
{
	width = w_;
	height = h_;

	bucket_width = bw;
	bucket_height = bh;

	center.setX(width * 0.5f);
	center.setY(height * 0.5f);

	for (int x = 0; x < width; x += bucket_width)
	{
		for (int y = 0; y < height; y += bucket_height)
		{
			buckets.push_back(new RenderBucket(x, y, x + bucket_width, y + bucket_height));
		}
	}

	sort(buckets.begin(), buckets.end(), BucketSorter());

	current_bucket = 0;
}

WorkGenerator::~WorkGenerator()
{
}

bool WorkGenerator::GetNextBucket(int &min_x, int &min_y, int &max_x, int &max_y)
{
	if (current_bucket > buckets.size() - 1)
	{
		return false;
	}

	min_x = buckets[current_bucket]->min_x;
	min_y = buckets[current_bucket]->min_y;
	max_x = buckets[current_bucket]->max_x;
	max_y = buckets[current_bucket]->max_y;

	current_bucket++;

	return true;
}

void WorkGenerator::Reset()
{
	current_bucket = 0;
}
