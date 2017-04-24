// ===============================================================================
//						  AGEIA PHYSX SDK TRAINING PROGRAMS
//							         UPDATE TIME
//
//						    Written by Bob Schade, 5-1-06
// ===============================================================================

#ifndef UPDATE_TIME_H
#define UPDATE_TIME_H

#include "Nx.h"

#include <windows.h>
#include <mmsystem.h>

float UpdateTime()
{
	static unsigned int previousTime = timeGetTime();
	unsigned int currentTime = timeGetTime();
	unsigned int elapsedTime = currentTime - previousTime;
	previousTime = currentTime;
	return (float)(elapsedTime)*0.001f;
}

#endif  // UPDATETIME_H
