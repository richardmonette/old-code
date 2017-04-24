
#pragma once

class Vector2 
{
public:

	Vector2() {}

	Vector2(float X, float Y) 
	{ 
		x = X; y = Y;
	}

	Vector2 operator+(Vector2 vVector)
	{
		return Vector2(vVector.x + x, vVector.y + y);
	}

	Vector2 operator-(Vector2 vVector)
	{
		return Vector2(x - vVector.x, y - vVector.y);
	}
	
	Vector2 operator*(float num)
	{
		return Vector2(x * num, y * num);
	}

	Vector2 operator/(float num)
	{
		return Vector2(x / num, y / num);
	}

	float x, y;
};