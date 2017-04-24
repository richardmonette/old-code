#include "IrradianceMap.h"

IrradianceMap::IrradianceMap(const int max_samp)
{
	stored_samples = 0;
	prev_scale = 1;
	max_samples = max_samp;

	samples = (IrradianceSample*)malloc( sizeof( IrradianceSample ) * ( max_samples+1 ) );

	if (samples == NULL) 
	{
		fprintf(stderr,"Out of memory initializing photon map\n");
		exit(-1);
	}

	bbox_min[0] = bbox_min[1] = bbox_min[2] = 1e8f;
	bbox_max[0] = bbox_max[1] = bbox_max[2] = -1e8f;
}

IrradianceMap::~IrradianceMap()
{
}

void IrradianceMap::nearest_precomputed_irradiance(float irradiance[3], const float pos[3], float normal[3])
{
	NearestSample np;
	np.pos[0] = pos[0]; np.pos[1] = pos[1]; np.pos[2] = pos[2];
	np.dist2 = 99999;
	
	locate_photon(&np, 1);

	irradiance[0] = np.index->power[0];
	irradiance[1] = np.index->power[1];
	irradiance[2] = np.index->power[2];

	normal[0] = np.index->normal[0];
	normal[1] = np.index->normal[1];
	normal[2] = np.index->normal[2];
}

void IrradianceMap::locate_photon(NearestSample *const np, const int index) const
{
	const IrradianceSample *p = &samples[index];
	float dist1;

	if (index < half_stored_samples) 
	{
		dist1 = np->pos[ p->plane ] - p->pos[ p->plane ];

		if (dist1>0.0) { // if dist1 is positive search right plane
			locate_photon( np, 2*index+1 );
			if ( dist1*dist1 < np->dist2 )
				locate_photon( np, 2*index );
			} else {         // dist1 is negative search left first
				locate_photon( np, 2*index );
			if ( dist1*dist1 < np->dist2 )
				locate_photon( np, 2*index+1 );
		}
	}

	// compute squared distance between current photon and np->pos

	dist1 = p->pos[0] - np->pos[0];
	float dist2 = dist1*dist1;
	dist1 = p->pos[1] - np->pos[1];
	dist2 += dist1*dist1;
	dist1 = p->pos[2] - np->pos[2];
	dist2 += dist1*dist1;
  
	if ( dist2 < np->dist2 ) 
	{
		np->index = p;
		np->dist2 = dist2;
	}
}


void IrradianceMap::store(
  const float power[3],
  const float pos[3],
  const float n[3])
//***************************
{
  if (stored_samples>=max_samples)
    return;

  stored_samples++;
  IrradianceSample *const node = &samples[stored_samples];

  for (int i=0; i<3; i++) {
    node->pos[i] = pos[i];

    if (node->pos[i] < bbox_min[i])
      bbox_min[i] = node->pos[i];
    if (node->pos[i] > bbox_max[i])
      bbox_max[i] = node->pos[i];

    node->power[i] = power[i];
  }

  node->normal.setX(n[0]);
  node->normal.setY(n[1]);
  node->normal.setZ(n[2]);
}



void IrradianceMap::balance(void)
{
  if (stored_samples>1) {
    // allocate two temporary arrays for the balancing procedure
    IrradianceSample **pa1 = (IrradianceSample**)malloc(sizeof(IrradianceSample*)*(stored_samples+1));
    IrradianceSample **pa2 = (IrradianceSample**)malloc(sizeof(IrradianceSample*)*(stored_samples+1));

    int i;
    for (i=0; i<=stored_samples; i++)
      pa2[i] = &samples[i];

    balance_segment( pa1, pa2, 1, 1, stored_samples );
    free(pa2);

    // reorganize balanced kd-tree (make a heap)
    int d, j=1, foo=1;
    IrradianceSample foo_photon = samples[j];

    for (i=1; i<=stored_samples; i++) {
      d=pa1[j]-samples;
      pa1[j] = NULL;
      if (d != foo)
        samples[j] = samples[d];
      else {
        samples[j] = foo_photon;

        if (i<stored_samples) {
          for (;foo<=stored_samples; foo++)
            if (pa1[foo] != NULL)
              break;
          foo_photon = samples[foo];
          j = foo;
        }
        continue;
      }
      j = d;
    }
    free(pa1);
  }

  half_stored_samples = stored_samples/2-1;
}


#define swap(ph,a,b) { IrradianceSample *ph2=ph[a]; ph[a]=ph[b]; ph[b]=ph2; }

// median_split splits the photon array into two separate
// pieces around the median with all photons below the
// the median in the lower half and all photons above
// than the median in the upper half. The comparison
// criteria is the axis (indicated by the axis parameter)
// (inspired by routine in "Algorithms in C++" by Sedgewick)
//*****************************************************************
void IrradianceMap::median_split(
  IrradianceSample **p,
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
void IrradianceMap::balance_segment(
  IrradianceSample **pbal,
  IrradianceSample **porg,
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
