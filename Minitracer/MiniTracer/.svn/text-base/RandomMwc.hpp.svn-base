/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef RandomMwc_h
#define RandomMwc_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * Simple, fast, good random number generator (Multiply-with-carry).<br/><br/>
 *
 * Perhaps the fastest of any generator that passes the Diehard tests.<br/><br/>
 *
 * Constant (sort-of: internally/non-semantically modifying).
 *
 * @implementation
 * Concatenation of following two 16-bit multiply-with-carry generators
 * x(n)=a*x(n-1)+carry mod 2^16 and y(n)=b*y(n-1)+carry mod 2^16, number and
 * carry packed within the same 32 bit integer. Algorithm recommended by
 * Marsaglia. Copyright (c) 2005, Glenn Rhoads.<br/><br/>
 *
 * <cite>http://web.archive.org/web/20050213041650/http://
 * paul.rutgers.edu/~rhoads/Code/code.html</cite>
 */
class RandomMwc
{
/// standard object services ---------------------------------------------------
public:
   explicit RandomMwc( dword seed = 0 );

// use defaults
//         ~RandomMwc();
//          RandomMwc( const RandomMwc& );
// RandomMwc& operator=( const RandomMwc& );


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           dword getDword()                                               const;
           float getFloat()                                               const;


/// fields ---------------------------------------------------------------------
private:
   mutable udword seeds_m[2];
};


}//namespace




#endif//RandomMwc_h
