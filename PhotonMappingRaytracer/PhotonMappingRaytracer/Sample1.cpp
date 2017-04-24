
#include "Sample1.h"
#include <time.h>

JitterSample1::JitterSample1(int _ns) : ns(_ns)
{
	random = new RandomMwc(time(0));
}

int JitterSample1::generate(float* samples)
{
   for (int i = 0; i < ns; i++)
	   samples[i] = float(i) / float(ns) + float((*random).getFloat()) / float(ns);
   return ns;
   
   // shuffle
   for (int i = ns-2; i >= 0; i--) 
   {
	   int target = int(float((*random).getFloat()) * float(i));
      float temp = samples[i+1];
      samples[i+1] = samples[target];
      samples[target] = temp;
   }
   return ns;

}
