
#include "Sample2.h"
#include <time.h>

JitterSample2::JitterSample2(int m, int n) : m(m), n(n) 
{
	random = new RandomMwc(time(NULL)); 
}

int JitterSample2::generate(Vector2 *a)  
{
    for (int i = 0; i < m; i++)
       for (int j = 0; j < n; j++)
		   a[i*n + j] = Vector2( (i + (*random).getFloat()) / m, (j + (*random).getFloat()) / n);
    
    // shuffle
    for (int i = m*n-2; i >= 0; i--) 
	{
        int target = int(float((*random).getFloat()) * float(i));
        Vector2 temp = a[i+1];
        a[i+1] = a[target];
        a[target] = temp;
    }
    return m*n;
}

HamSample2::HamSample2(int num) 
{
	n = num; 
	random = new RandomMwc(time(NULL)); 
}

int HamSample2::generate(Vector2 *a)  
{
	float p, u, v;
	int k, kk;
	float offu = (*random).getFloat(); 
	float offv = (*random).getFloat(); 
  
	for (k=0 ; k<n ; k++)
	{
		u = 0.0f;
		p = 0.5f;
		for (kk=k ; kk ; p*=0.5, kk>>=1)
			if (kk & 1) u += p; // kk mod 2 == 1 

		v = ((float)k + 0.5) / (float)n;
		u += offu;
		v += offv;
		u -= int(u);
		v -= int(v);
		a[k] = Vector2(u, v);
	}

	return n;
}
 
