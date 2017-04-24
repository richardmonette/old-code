
#pragma once

#if (defined(__linux__) && defined(__i386__)) || defined(WIN32)
#define FAST_INT 1
#endif
#define _doublemagicroundeps	      (.5-1.4e-11)
//almost .5f = .5f - 1e^(number of exp bit)

inline int Round2Int(double val) 
{
	#ifdef FAST_INT
	#define _doublemagic			double (6755399441055744.0)
		//2^52 * 1.5,  uses limited precision to floor
		val		= val + _doublemagic;
		return ((long*)&val)[0];
	#else
		return int (val+_doublemagicroundeps);
	#endif
}

inline int Float2Int(double val) 
{
	#ifdef FAST_INT
		return (val<0) ?  Round2Int(val+_doublemagicroundeps) :
			Round2Int(val-_doublemagicroundeps);
	#else
		return (int)val;
	#endif
}

inline int Floor2Int(double val) 
{
	#ifdef FAST_INT
		return Round2Int(val - _doublemagicroundeps);
	#else
		return (int)floorf(val);
	#endif
}

inline int Ceil2Int(double val) 
{
	#ifdef FAST_INT
		return Round2Int(val + _doublemagicroundeps);
	#else
		return (int)ceilf(val);
	#endif
}