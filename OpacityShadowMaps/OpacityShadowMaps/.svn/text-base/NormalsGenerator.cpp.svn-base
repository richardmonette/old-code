//----------------------------------------------------------------------------
// photonmap.cc
// An example implementation of the photon map data structure
//
// Henrik Wann Jensen - February 2001
//----------------------------------------------------------------------------

#include "NormalsGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

/* This is the constructor for the photon map.
 * To create the photon map it is necessary to specify the
 * maximum number of photons that will be stored
*/
//************************************************
NormalsGenerator :: NormalsGenerator( const int max_phot )
//************************************************ 
{
  stored_points = 0;
  prev_scale = 1;
  max_points = max_phot;

  points = (Point*)malloc( sizeof( Point ) * ( max_points+1 ) );

  if (points == NULL) {
    fprintf(stderr,"Out of memory initializing photon map\n");
    exit(-1);
  }

  bbox_min[0] = bbox_min[1] = bbox_min[2] = 1e8f;
  bbox_max[0] = bbox_max[1] = bbox_max[2] = -1e8f;
  
}

//*************************
NormalsGenerator :: ~NormalsGenerator()
//*************************
{
  free( points );
}

/* irradiance_estimate computes an irradiance estimate
 * at a given surface position
*/
//**********************************************
void NormalsGenerator :: normal_estimate(
  float normal[3],                // returned irradiance
  const float pos[3],            // surface position
  const float max_dist,          // max distance to look for photons
  const int npoints )      // number of photons to use
//**********************************************
{
	normal[0] = normal[1] = normal[2] = 0.0;

	NearestPoints np;
	np.dist2 = (float*)malloc( sizeof(float)*(npoints+1) );
	np.index = (Point**)malloc( sizeof(Point*)*(npoints+1) );

	np.pos[0] = pos[0]; np.pos[1] = pos[1]; np.pos[2] = pos[2];
	np.max = npoints;
	np.found = 0;
	np.got_heap = 0;
	np.dist2[0] = max_dist*max_dist;

	// locate the nearest photons
	locate_points( &np, 1 );

	//fprintf(stdout, "Located: %d on distance %f\n", np.found, np.dist2[0]);

	// if less than 8 photons return
	if (np.found<8)
	{
		free(np.dist2);
		free(np.index);
		return;
	}

	// sum irradiance from all photons
	for (int i=1; i<=np.found; i++) 
	{
		const Point *p = np.index[i];

		normal[0] += p->pos[0] - pos[0];
		normal[1] += p->pos[1] - pos[1];
		normal[2] += p->pos[2] - pos[2];
	}

	free(np.dist2);
	free(np.index);
}

/* locate_photons finds the nearest photons in the
 * photon map given the parameters in np
*/
//******************************************
void NormalsGenerator :: locate_points(
  NearestPoints *const np,
  const int index ) const
//******************************************
{
  Point *p = &points[index];
  float dist1;

  if (index<half_stored_points) {
    dist1 = np->pos[ p->plane ] - p->pos[ p->plane ];

    if (dist1>0.0) { // if dist1 is positive search right plane
      locate_points( np, 2*index+1 );
      if ( dist1*dist1 < np->dist2[0] )
        locate_points( np, 2*index );
    } else {         // dist1 is negative search left first
      locate_points( np, 2*index );
      if ( dist1*dist1 < np->dist2[0] )
        locate_points( np, 2*index+1 );
    }
  }

  // compute squared distance between current photon and np->pos

  dist1 = p->pos[0] - np->pos[0];
  float dist2 = dist1*dist1;
  dist1 = p->pos[1] - np->pos[1];
  dist2 += dist1*dist1;
  dist1 = p->pos[2] - np->pos[2];
  dist2 += dist1*dist1;
  
  if ( dist2 < np->dist2[0] ) 
  {
    // we found a photon  [:)] Insert it in the candidate list

    if ( np->found < np->max ) {
      // heap is not full; use array
      np->found++;
      np->dist2[np->found] = dist2;
      np->index[np->found] = p;
    } else {
      int j,parent;

      if (np->got_heap==0) { // Do we need to build the heap?
        // Build heap
        float dst2;
        Point *phot;
        int half_found = np->found>>1;
        for ( int k=half_found; k>=1; k--) {
          parent=k;
          phot = np->index[k];
          dst2 = np->dist2[k];
          while ( parent <= half_found ) {
            j = parent+parent;
            if (j<np->found && np->dist2[j]<np->dist2[j+1])
              j++;
            if (dst2>=np->dist2[j])
              break;
            np->dist2[parent] = np->dist2[j];
            np->index[parent] = np->index[j];
            parent=j;
          }
          np->dist2[parent] = dst2;
          np->index[parent] = phot;
        }
        np->got_heap = 1;
      }

      // insert new photon into max heap
      // delete largest element, insert new and reorder the heap

      parent=1;
      j = 2;
      while ( j <= np->found ) {
        if ( j < np->found && np->dist2[j] < np->dist2[j+1] )
          j++;
        if ( dist2 > np->dist2[j] )
          break;
        np->dist2[parent] = np->dist2[j];
        np->index[parent] = np->index[j];
        parent = j;
        j += j;
      }
      np->index[parent] = p;
      np->dist2[parent] = dist2;

      np->dist2[0] = np->dist2[1];
    }
  }
}


/* store puts a photon into the flat array that will form
 * the final kd-tree.
 *
 * Call this function to store a photon.
*/
//***************************
void NormalsGenerator :: store(int vertIndex,
			   const float pos[3])
//***************************
{
  if (stored_points>=max_points)
    return;

  stored_points++;
  Point *const node = &points[stored_points];

  for (int i=0; i<3; i++) {
    node->pos[i] = pos[i];

    if (node->pos[i] < bbox_min[i])
      bbox_min[i] = node->pos[i];
    if (node->pos[i] > bbox_max[i])
      bbox_max[i] = node->pos[i];

  }

  node->vertIndex = vertIndex;

  node->power = 0;

}




/* balance creates a left balanced kd-tree from the flat photon array.
 * This function should be called before the photon map
 * is used for rendering.
 */
//******************************
void NormalsGenerator :: balance(void)
//******************************
{
  if (stored_points>1) {
    // allocate two temporary arrays for the balancing procedure
    Point **pa1 = (Point**)malloc(sizeof(Point*)*(stored_points+1));
    Point **pa2 = (Point**)malloc(sizeof(Point*)*(stored_points+1));

    int i;
    for (i=0; i<=stored_points; i++)
      pa2[i] = &points[i];

    balance_segment( pa1, pa2, 1, 1, stored_points );
    free(pa2);

    // reorganize balanced kd-tree (make a heap)
    int d, j=1, foo=1;
    Point foo_point = points[j];

    for (i=1; i<=stored_points; i++) {
      d=pa1[j]-points;
      pa1[j] = NULL;
      if (d != foo)
        points[j] = points[d];
      else {
        points[j] = foo_point;

        if (i<stored_points) {
          for (;foo<=stored_points; foo++)
            if (pa1[foo] != NULL)
              break;
          foo_point = points[foo];
          j = foo;
        }
        continue;
      }
      j = d;
    }
    free(pa1);
  }

  half_stored_points = stored_points/2-1;
}


#define swap(ph,a,b) { Point *ph2=ph[a]; ph[a]=ph[b]; ph[b]=ph2; }

// median_split splits the photon array into two separate
// pieces around the median with all photons below the
// the median in the lower half and all photons above
// than the median in the upper half. The comparison
// criteria is the axis (indicated by the axis parameter)
// (inspired by routine in "Algorithms in C++" by Sedgewick)
//*****************************************************************
void NormalsGenerator :: median_split(
  Point **p,
  const int start,               // start of photon block in array
  const int end,                 // end of photon block in array
  const int median,              // desired median number
  const int axis )               // axis to split along
//*****************************************************************
{
  int left = start;
  int right = end;

  while ( right > left ) {
    const float v = p[right]->pos[axis];
    int i=left-1;
    int j=right;
    for (;;) {
      while ( p[++i]->pos[axis] < v )
        ;
      while ( p[--j]->pos[axis] > v && j>left )
        ;
      if ( i >= j )
        break;
      swap(p,i,j);
    }

    swap(p,i,right);
    if ( i >= median )
      right=i-1;
    if ( i <= median )
      left=i+1;
  }
}

  
// See "Realistic image synthesis using Photon Mapping" chapter 6
// for an explanation of this function
//****************************
void NormalsGenerator :: balance_segment(
  Point **pbal,
  Point **porg,
  const int index,
  const int start,
  const int end )
//****************************
{
  //--------------------
  // compute new median
  //--------------------

  int median=1;
  while ((4*median) <= (end-start+1))
    median += median;

  if ((3*median) <= (end-start+1)) {
    median += median;
    median += start-1;
  } else    
    median = end-median+1;

  //--------------------------
  // find axis to split along
  //--------------------------

  int axis=2;
  if ((bbox_max[0]-bbox_min[0])>(bbox_max[1]-bbox_min[1]) &&
(bbox_max[0]-bbox_min[0])>(bbox_max[2]-bbox_min[2]))
    axis=0;
  else if ((bbox_max[1]-bbox_min[1])>(bbox_max[2]-bbox_min[2]))
    axis=1;

  //------------------------------------------
  // partition photon block around the median
  //------------------------------------------

  median_split( porg, start, end, median, axis );

  pbal[ index ] = porg[ median ];
  pbal[ index ]->plane = axis;

  //----------------------------------------------
  // recursively balance the left and right block
  //----------------------------------------------

  if ( median > start ) {
    // balance left segment
    if ( start < median-1 ) {
      const float tmp=bbox_max[axis];
      bbox_max[axis] = pbal[index]->pos[axis];
      balance_segment( pbal, porg, 2*index, start, median-1 );
      bbox_max[axis] = tmp;
    } else {
      pbal[ 2*index ] = porg[start];
    }
  }

  if ( median < end ) {
    // balance right segment
    if ( median+1 < end ) {
      const float tmp = bbox_min[axis];     
      bbox_min[axis] = pbal[index]->pos[axis];
      balance_segment( pbal, porg, 2*index+1, median+1, end );
      bbox_min[axis] = tmp;
    } else {
      pbal[ 2*index+1 ] = porg[end];
    }
  } 
}

