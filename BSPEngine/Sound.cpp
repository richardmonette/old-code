#include "Sound.h"

Sound::Sound()
{
	alutInit(0, 0);
	alGetError();

	SourcesPos[0][0] = -175;
    SourcesPos[0][1] = 162;
    SourcesPos[0][2] = -247;

	SourcesVel[0][0] = 0;
    SourcesVel[0][1] = 0;
    SourcesVel[0][2] = 0;

	SourcesPos[1][0] = 391;
    SourcesPos[1][1] = 122;
    SourcesPos[1][2] = 48;

	SourcesVel[1][0] = 0;
    SourcesVel[1][1] = 0;
    SourcesVel[1][2] = 0;

	SourcesPos[2][0] = 0;
    SourcesPos[2][1] = 103;
    SourcesPos[2][2] = 431;

	SourcesVel[2][0] = 0;
    SourcesVel[2][1] = 0;
    SourcesVel[2][2] = 0;

	LoadALData();

	alSourcePlay(Sources[AMBIENT_0]);
	alSourcePlay(Sources[AMBIENT_1]);
	alSourcePlay(Sources[AMBIENT_2]);
}

Sound::~Sound()
{
	alDeleteBuffers(NUM_BUFFERS, Buffers);
	alDeleteSources(NUM_SOURCES, Sources);
	alutExit();
}

Vector3 previousP;
void Sound::SetListenerPosition(Vector3 &p, Vector3 &up)
{
	ListenerPos[0] = p.x;
	ListenerPos[1] = p.y;
	ListenerPos[2] = p.z;

	ListenerOri[0] = p.x;
	ListenerOri[1] = p.y;
	ListenerOri[2] = p.z;

	ListenerVel[0] = p.x - previousP.x;
	ListenerVel[1] = p.y - previousP.y;
	ListenerVel[2] = p.z - previousP.z;

	previousP = p;

	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

ALboolean Sound::LoadALData()
{
	// Variables to load into.

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into buffers.

	alGenBuffers(NUM_BUFFERS, Buffers);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alutLoadWAVFile("sound/ambient0.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[AMBIENT_0], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/ambient1.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[AMBIENT_1], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/ambient2.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[AMBIENT_2], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind buffers into audio sources.

	alGenSources(NUM_SOURCES, Sources);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alSourcei (Sources[AMBIENT_0], AL_BUFFER,   Buffers[AMBIENT_0]   );
	alSourcef (Sources[AMBIENT_0], AL_PITCH,    1.0f              );
	alSourcef (Sources[AMBIENT_0], AL_GAIN,     1.0f              );
	alSourcefv(Sources[AMBIENT_0], AL_POSITION, SourcesPos[AMBIENT_0]);
	alSourcefv(Sources[AMBIENT_0], AL_VELOCITY, SourcesVel[AMBIENT_0]);
	alSourcei (Sources[AMBIENT_0], AL_LOOPING,  AL_TRUE           );
	alSourcef (Sources[AMBIENT_0], AL_REFERENCE_DISTANCE,  25          );

	alSourcei (Sources[AMBIENT_1], AL_BUFFER,   Buffers[AMBIENT_1]   );
	alSourcef (Sources[AMBIENT_1], AL_PITCH,    1.0f            );
	alSourcef (Sources[AMBIENT_1], AL_GAIN,     1.0f            );
	alSourcefv(Sources[AMBIENT_1], AL_POSITION, SourcesPos[AMBIENT_1]);
	alSourcefv(Sources[AMBIENT_1], AL_VELOCITY, SourcesVel[AMBIENT_1]);
	alSourcei (Sources[AMBIENT_1], AL_LOOPING,  AL_TRUE        );
	alSourcef (Sources[AMBIENT_1], AL_REFERENCE_DISTANCE,  25         );

	alSourcei (Sources[AMBIENT_2], AL_BUFFER,   Buffers[AMBIENT_2]   );
	alSourcef (Sources[AMBIENT_2], AL_PITCH,    1.0f            );
	alSourcef (Sources[AMBIENT_2], AL_GAIN,     1.0f            );
	alSourcefv(Sources[AMBIENT_2], AL_POSITION, SourcesPos[AMBIENT_2]);
	alSourcefv(Sources[AMBIENT_2], AL_VELOCITY, SourcesVel[AMBIENT_2]);
	alSourcei (Sources[AMBIENT_2], AL_LOOPING,  AL_TRUE        );
	alSourcef (Sources[AMBIENT_2], AL_REFERENCE_DISTANCE,  25          );

	// Do another error check and return.

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	return AL_TRUE;
}
