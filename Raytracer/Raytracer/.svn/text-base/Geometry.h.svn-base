
#pragma once

inline Vector Normalize(const Vector &v)
{
	return v / v.Length();
}

inline Vector Cross(const Vector &v1, const Vector &v2)
{
	return Vector((v1.y * v2.z) - (v1.z * v2.y),
				  (v1.z * v2.x) - (v1.x * v2.z),
				  (v1.x * v2.y) - (v1.y * v2.x));
}