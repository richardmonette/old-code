/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#include "Vector3f.hpp"
#include "Triangle.hpp"

#include "SpatialIndex.hpp"


using namespace hxa7241_minilight;




/// constants ------------------------------------------------------------------
namespace
{
   // accommodates scene including sun and earth, down to cm cells
   // (use 47 for mm)
   const dword  MAX_LEVELS = 44;
   const udword MAX_ITEMS  =  8;
}




/// standard object services ---------------------------------------------------
SpatialIndex::SpatialIndex
(
   const Vector3f&         eyePosition,
   const vector<Triangle>& items
)
{
   // set overall bound (and convert to collection of pointers)
   vector<const Triangle*> itemPs( items.size() );
   {
      // accommodate eye position
      // (makes tracing algorithm simpler)
      for( int i = 6;  i-- > 0;  bound_m[i] = eyePosition[i % 3] ) {}

      // accommodate all items
      for( int i = items.size();  i-- > 0;  itemPs[i] = &(items[i]) )
      {
         float itemBound[6];
         items[i].getBound( itemBound );

         // accommodate item
         for( int j = 0;  j < 6;  ++j )
         {
            if((bound_m[j] > itemBound[j]) ^ (j > 2)) bound_m[j] = itemBound[j];
         }
      }

      // make cubical
      float maxSize = 0.0f;
      for( const float* b = bound_m + 3;  b-- > bound_m; )
      {
         if( maxSize < (b[3] - b[0]) ) maxSize = b[3] - b[0];
      }
      for( float* b = bound_m + 3;  b-- > bound_m; )
      {
         if( b[3] < (b[0] + maxSize) ) b[3] = b[0] + maxSize;
      }
   }

   // make cell tree
   SpatialIndex::construct( itemPs, 0 );
}


SpatialIndex::SpatialIndex
(
   const float bound[6]
)
{
   for( int i = 6;  i-- > 0;  bound_m[i] = bound[i] ) {}
}


SpatialIndex::~SpatialIndex()
{
   // only delete branch sub-parts
   for( int i = vector_m.size(); isBranch_m & (i-- > 0); )
   {
      delete static_cast<const SpatialIndex*>( vector_m[i] );
   }
}




/// commands -------------------------------------------------------------------




/// queries --------------------------------------------------------------------
void SpatialIndex::getIntersection
(
   const Vector3f&  rayOrigin,
   const Vector3f&  rayDirection,
   const void*      lastHit,
   const Triangle*& pHitObject,
   Vector3f&        hitPosition,
   Vector3f&        hitUVT,
   const Vector3f*  pStart
) const
{
   // is branch: step through subcells and recurse
   if( isBranch_m )
   {
      pStart = pStart ? pStart : &rayOrigin;

      // find which subcell holds ray origin (ray origin is inside cell)
      dword subCell = 0;
      for( int i = 3;  i-- > 0; )
      {
         // compare dimension with center
         subCell |= ((*pStart)[i] >= ((bound_m[i] + bound_m[i+3]) * 0.5f)) << i;
      }

      // step through intersected subcells
      for( Vector3f cellPosition( *pStart );  ; )
      {
         if( 0 != vector_m[subCell] )
         {
            // intersect subcell
            static_cast<const SpatialIndex*>( vector_m[subCell]
               )->getIntersection( rayOrigin, rayDirection, lastHit, pHitObject,
               hitPosition, hitUVT, &cellPosition );
            // exit if item hit
            if( 0 != pHitObject ) break;
         }

         // find next subcell ray moves to
         // (by finding which face of the corner ahead is crossed first)
         dword axis = 2;
         float step[3];
         for( int i = 3;  i-- > 0;  axis = step[i] < step[axis] ? i : axis )
         {
            const dword high = (subCell >> i) & 1;
            const float face = (rayDirection[i] < 0.0f) ^ high ?
               bound_m[i + (high * 3)] : (bound_m[i] + bound_m[i + 3]) * 0.5f;
            // distance to face
            // (div by zero produces infinity, which is later discarded)
            step[i] = (face - rayOrigin[i]) / rayDirection[i];
         }

         // leaving branch if: subcell is low and direction is negative,
         // or subcell is high and direction is positive
         if( ((subCell >> axis) & 1) ^ (rayDirection[axis] < 0.0f) ) break;

         // move to (outer face of) next subcell
         cellPosition = rayOrigin + (rayDirection * step[axis]);
         subCell      = subCell ^ (1 << axis);
      }
   }
   // is leaf: exhaustively intersect contained items
   else
   {
      pHitObject = 0;

      float nearestDistance = FLOAT_MAX;

      // step through items
      for( int i = vector_m.size();  i-- > 0; )
      {
         const Triangle& item = *static_cast<const Triangle*>(vector_m[i]);

         // avoid false intersection with surface just come from
         if( &item != lastHit )
         {
            // intersect ray with item, and inspect if nearest so far
            float distance = FLOAT_MAX;
            if( item.getIntersection( rayOrigin, rayDirection, distance, hitUVT ) &&
               (distance < nearestDistance) )
            {
               // check intersection is inside cell bound (with tolerance)
               const Vector3f hit( rayOrigin + (rayDirection * distance) );
               const float t = Triangle::TOLERANCE();
               if( (bound_m[0] - hit[0] <= t) & (hit[0] - bound_m[3] <= t) &
                   (bound_m[1] - hit[1] <= t) & (hit[1] - bound_m[4] <= t) &
                   (bound_m[2] - hit[2] <= t) & (hit[2] - bound_m[5] <= t) )
               {
                  pHitObject      = &item;
                  nearestDistance = distance;
                  hitPosition     = hit;
               }
            }
         }
      }
   }
}




/// implementation -------------------------------------------------------------
const SpatialIndex* SpatialIndex::construct
(
   const vector<const Triangle*>& items,
   const dword                    level
)
{
   // is branch if items overflow leaf and tree not too deep
   isBranch_m = (items.size() > MAX_ITEMS) & (level < (MAX_LEVELS - 1));

   // be branch: make sub-cells, and recurse construction
   if( isBranch_m )
   {
      // make subcells
      for( int s = (vector_m.resize( 8 ), vector_m.size()), q = 0;  s-- > 0; )
      {
         // make subcell bound
         // collect items that overlap subcell
         float subBound[6];
         vector<const Triangle*> subItems;
         for( int i = items.size();  i-- > 0; )
         {
            float itemBound[6];
            items[i]->getBound( itemBound );

            int isOverlap = 1;
            // step through two coord sets
            for( int j = 0, d = 0, m = 0;  j < 6;  ++j, d = j / 3, m = j % 3 )
            {
               // make subcell bound
               subBound[j] = ((s >> m) & 1) ^ d ?
                  (bound_m[m] + bound_m[m + 3]) * 0.5f : bound_m[j];
               // must overlap in all dimensions
               isOverlap &= (itemBound[(d ^ 1) * 3 + m] >= subBound[j]) ^ d;
            }

            if( isOverlap ) subItems.push_back( items[i] );
         }

         // curtail degenerate subdivision by adjusting next level
         // (degenerate if two or more subcells copy entire contents of parent,
         // or if subdivision reaches below mm size)
         // (having a model including the sun requires one subcell copying
         // entire contents of parent to be allowed)
         q += subItems.size() == items.size() ? 1 : 0;
         const dword nextLevel = (q > 1) | ((subBound[3] - subBound[0]) <
            (Triangle::TOLERANCE() * 4.0f)) ? MAX_LEVELS : level + 1;

         // recurse
         vector_m[s] = !subItems.empty() ? (new SpatialIndex( subBound )
            )->construct( subItems, nextLevel ) : 0;
      }
   }
   // be leaf: store items
   else
   {
      // (trim reserve capacity, since vector_m was default-empty)
      vector_m = *reinterpret_cast<const vector<const void*>*>( &items );
   }

   return this;
}
