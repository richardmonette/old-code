/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef Vector3f_h
#define Vector3f_h


#include <iosfwd>




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * Yes, its the 3D vector class!.<br/><br/>
 *
 * ...mostly the usual sort of stuff.<br/><br/>
 *
 * (Unused methods are commented out. They do work fine though.)<br/><br/>
 *
 * Constant.
 */
class Vector3f
{
/// standard object services ---------------------------------------------------
public:
            Vector3f();
            Vector3f( float x,
                      float y,
                      float z );
// explicit Vector3f( const float xyz[3] );

// use defaults
//         ~Vector3f();
//          Vector3f( const Vector3f& );
// Vector3f& operator=( const Vector3f& );


/// commands -------------------------------------------------------------------
//         Vector3f& set( float x,
//                        float y,
//                        float z );
//         Vector3f& set( const float xyz[3] );

//         Vector3f& negateEq ();
//         Vector3f& absEq    ();
//         Vector3f& unitizeEq();
//         Vector3f& crossEq  ( const Vector3f& );

//         Vector3f& operator+=( const Vector3f& );
//         Vector3f& operator-=( const Vector3f& );
//         Vector3f& operator*=( const Vector3f& );
//         Vector3f& operator/=( const Vector3f& );
//         Vector3f& operator*=( float );
//         Vector3f& operator/=( float );

//         Vector3f& clampMin( const Vector3f& );
//         Vector3f& clampMax( const Vector3f& );
//         Vector3f& clamp   ( const Vector3f& min,
//                             const Vector3f& max );
//         /**
//          * 0 to almost 1, ie: [0,1).
//          */
//         Vector3f& clamp01();


/// queries --------------------------------------------------------------------
//         void      get( float& x,
//                        float& y,
//                        float& z )                                      const;
//         void      get( float xyz[3] )                                  const;

           float     getX()                                               const;
           float     getY()                                               const;
//         float     getZ()                                               const;
           float     operator[]( dword )                                  const;

		   void      setX(float x);
		   void      setY(float y);

//         float     sum()                                                const;
//         float     average()                                            const;
//         float     smallest()                                           const;
//         float     largest()                                            const;

//         float     length()                                             const;
           float     dot( const Vector3f& )                               const;
//         float     distance( const Vector3f& )                          const;

           Vector3f  operator-()                                          const;
//         Vector3f  abs()                                                const;
           Vector3f  unitize()                                            const;
           Vector3f  cross( const Vector3f& )                             const;

           Vector3f  operator+( const Vector3f& )                         const;
           Vector3f  operator-( const Vector3f& )                         const;
           Vector3f  operator*( const Vector3f& )                         const;
//         Vector3f  operator/( const Vector3f& )                         const;
           Vector3f  operator*( float )                                   const;
           Vector3f  operator/( float )                                   const;

//         bool      operator==( const Vector3f& )                        const;
//         bool      operator!=( const Vector3f& )                        const;
           bool      isZero()                                             const;

//         // returning vectors of -1.0f or 0.0f or +1.0f
//         Vector3f  sign   ()                                            const;
//         Vector3f  compare( const Vector3f& )                           const;
//
//         // returning vectors of static_cast<float>(bool)
//         Vector3f  equal     ( const Vector3f& )                        const;
//         Vector3f  operator> ( const Vector3f& )                        const;
//         Vector3f  operator>=( const Vector3f& )                        const;
//         Vector3f  operator< ( const Vector3f& )                        const;
//         Vector3f  operator<=( const Vector3f& )                        const;

//         /**
//          * 0 to almost 1, ie: [0,1).
//          */
//         void      getClamped01( Vector3f& result )                     const;
           Vector3f  getClamped  ( const Vector3f& min,
                                   const Vector3f& max )                  const;


/// friends --------------------------------------------------------------------
// friend  Vector3f  operator*( float, const Vector3f& );
// friend  Vector3f  operator/( float, const Vector3f& );


/// constants ------------------------------------------------------------------
   static const Vector3f& ZERO();
// static const Vector3f& HALF();
   static const Vector3f& ONE();
// static const Vector3f& EPSILON();
// static const Vector3f& ALMOST_ONE();
   static const Vector3f& MIN();
   static const Vector3f& MAX();
// static const Vector3f& SMALL();
// static const Vector3f& LARGE();
// static const Vector3f& X();
// static const Vector3f& Y();
// static const Vector3f& Z();


/// fields ---------------------------------------------------------------------
private:
   float xyz_m[3];
};








/// friends
//Vector3f operator*( float, const Vector3f& );
//Vector3f operator/( float, const Vector3f& );




/// streaming
//std::ostream& operator<<( std::ostream&, const Vector3f& );
std::istream& operator>>( std::istream&,       Vector3f& );




/// INLINES ///


/// standard object services ---------------------------------------------------
//inline
//Vector3f::Vector3f
//(
//   const float xyz[3]
//)
//{
//   Vector3f::set( xyz );
//}


//inline
//Vector3f::~Vector3f()
//{
//}
//
//
//inline
//Vector3f::Vector3f
//(
//   const Vector3f& other
//)
//{
//   Vector3f::operator=( other );
//}




/// queries --------------------------------------------------------------------
inline
float Vector3f::getX() const
{
   return xyz_m[0];
}
//
//
inline
float Vector3f::getY() const
{
   return xyz_m[1];
}
//
//
//inline
//float Vector3f::getZ() const
//{
//   return xyz_m[2];
//}


inline
void Vector3f::setX(float x)
{
	xyz_m[0] = x;
}

inline
void Vector3f::setY(float y)
{
	xyz_m[1] = y;
}

inline
float Vector3f::operator[]
(
   const dword i
) const
{
   return xyz_m[i];
}


}//namespace




#endif//Vector3f_h
