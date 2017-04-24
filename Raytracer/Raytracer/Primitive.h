#pragma once

#include <vector>
#include "ReferenceCounted.h"
#include "Reference.h"
#include "DifferentialGeometry.h"
#include "BBox.h"

class Ray;
class Intersection;

class Primitive :
	public ReferenceCounted
{
public:
	Primitive();
	~Primitive();

	virtual BBox WorldBound() const = 0;
	virtual bool CanIntersect() const;
	virtual bool Intersect(const Ray &r, Intersection *in) const = 0;
	virtual bool IntersectP(const Ray &r) const = 0;
	virtual bool Refine(vector<Reference<Primitive>> &refined) const;
	void FullyRefine(vector<Reference<Primitive>> &refined) const;
};

struct Intersection
{
	void FullyRefine(vector<Reference<Primitive>> &refined) const;

	DifferentialGeometry dg;
	const Primitive *primitive;
	Transform WorldToObject;
};

void Primitive::FullyRefine(vector<Reference<Primitive>> &refined) const
{
	vector<Reference<Primitive>> todo;
	todo.push_back(const_cast<Primitive*>(this));
	while(todo.size())
	{
		Reference<Primitive> prim = todo.back();
		todo.pop_back();
		if (prim->CanIntersect())
		{
			refined.push_back(prim);
		}
		else
		{
			prim->Refine(todo);
		}
	}
}
