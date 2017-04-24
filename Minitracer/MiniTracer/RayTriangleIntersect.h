
#pragma once

#include "Global.h"

inline bool RayTriangleIntersect(Vec *o, Vec *d, Vec *p0, Vec *p1, Vec *p2, double *u, double *v, double *t)
{
	Vec edge1( *p1 - *p0 );
	Vec edge2( *p2 - *p0 );

	Vec pvec( cross( *d, edge2 ) );

	double det = dot(edge1, pvec );

	{
	  static const double EPSILON = 0.000001f;
	  if( (det > -EPSILON) & (det < EPSILON) )
	  {
		 return false;
	  }

	  double inv_det = 1.0f / det;

	  Vec tvec( *o - *p0 );

	  *u = dot( tvec, pvec ) * inv_det;
	  if( (*u < 0.0f) | (*u > 1.0f) )
	  {
		 return false;
	  }

	  Vec qvec( cross( tvec, edge1 ) );

	  *v = dot( *d, qvec ) * inv_det;
	  if( (*v < 0.0f) | (*u + *v > 1.0f) )
	  {
		 return false;
	  }

	  *t = dot( edge2, qvec ) * inv_det;
	}

	return *t >= 0.0f;
}