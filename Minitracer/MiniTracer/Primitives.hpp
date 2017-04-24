/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef Primitives_h
#define Primitives_h


//#include <limits.h>
#include <float.h>




namespace hxa7241
{

/// types ----------------------------------------------------------------------

//   typedef  signed   char   byte;
   typedef  unsigned char   ubyte;

//   typedef  signed   short  word;
//   typedef  unsigned short  uword;

   typedef  signed   int    dword;
   typedef  unsigned int    udword;


/// constants ------------------------------------------------------------------
//   const byte   BYTE_MIN    = SCHAR_MIN;
//   const byte   BYTE_MAX    = SCHAR_MAX;
//   const int    BYTE_BITS   = 8;
//
//   const ubyte  UBYTE_MIN   = 0;
//   const ubyte  UBYTE_MAX   = UCHAR_MAX;
//   const int    UBYTE_BITS  = 8;
//
//
//   const word   WORD_MIN    = SHRT_MIN;
//   const word   WORD_MAX    = SHRT_MAX;
//   const int    WORD_BITS   = 16;
//
//   const uword  UWORD_MIN   = 0;
//   const uword  UWORD_MAX   = USHRT_MAX;
//   const int    UWORD_BITS  = 16;
//
//
//   const dword  DWORD_MIN   = INT_MIN;
//   const dword  DWORD_MAX   = INT_MAX;
//   const int    DWORD_BITS  = 32;
//
//   const udword UDWORD_MIN  = 0;
//   const udword UDWORD_MAX  = UINT_MAX;
//   const int    UDWORD_BITS = 32;


//   const float  FLOAT_MIN_POS    = static_cast<float>(FLT_MIN);
   const float  FLOAT_MIN_NEG    = static_cast<float>(-FLT_MAX);
   const float  FLOAT_MAX        = static_cast<float>(FLT_MAX);
//   const float  FLOAT_EPSILON    = static_cast<float>(FLT_EPSILON);
//   const float  FLOAT_ALMOST_ONE = static_cast<float>(1.0f - FLT_EPSILON);
//   const float  FLOAT_SMALL      = static_cast<float>(1.0e-12f);
//   const float  FLOAT_LARGE      = static_cast<float>(1.0e+12f);

}//namespace




#endif//Primitives_h
