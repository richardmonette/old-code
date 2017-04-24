#pragma once

#include "al/al.h"
#include "al/alc.h"
#include "al/alut.h"
#include "Vector3.h"

#define NUM_BUFFERS 3
#define NUM_SOURCES 3

#define AMBIENT_0	0
#define AMBIENT_1   1
#define AMBIENT_2   2

// OpenAL code based on the following:
// <cite> Jesse Maurais, OpenAL Lesson 3: Multiple Sources http://www.devmaster.net/articles/openal-tutorials/lesson3.php </cite>

class Sound
{
public:
	Sound();
	~Sound();

	ALboolean LoadALData();

	void SetListenerPosition(Vector3 &p, Vector3 &up);

	ALuint Buffers[NUM_BUFFERS];
	ALuint Sources[NUM_SOURCES];

	ALfloat SourcesPos[NUM_SOURCES][3];
	ALfloat SourcesVel[NUM_SOURCES][3];

	ALfloat ListenerPos[3];
	ALfloat ListenerVel[3];
	ALfloat ListenerOri[3];
};

