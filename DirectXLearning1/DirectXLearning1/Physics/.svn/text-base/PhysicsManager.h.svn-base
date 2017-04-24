#pragma once

#include "../Global.h"

#include "NxPhysics.h"

class PhysicsManager
{
public:

	static PhysicsManager* GetInstance();
	~PhysicsManager();

	void Initialize();
	void Deinitialize();

	void Update();

	// Physics SDK globals
	NxPhysicsSDK * m_PhysicsSDK;
	NxScene * m_Scene;
	NxVec3 m_DefaultGravity;

	NxReal m_DeltaTime;

	void InitNx();
	void ReleaseNx();
	void ResetNx();

	void StartPhysics();
	void GetPhysicsResults();

	NxActor * CreateBox(NxVec3 * size, bool isStatic);
	NxActor * CreateGroundPlane();

private:

	static PhysicsManager * pInstance;

};