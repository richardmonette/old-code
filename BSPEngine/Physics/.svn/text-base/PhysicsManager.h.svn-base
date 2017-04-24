#pragma once

#include "NxPhysics.h"
//#include "../types.h"
#include "../Entity.h"
#include "../EntityManager.h"
#include "../Camera.h"

#include "NxCapsuleController.h"
#include <NxControllerManager.h>
#include "NxCharacter.h"
#include "NxUserAllocatorDefault.h"

#include <iostream>

enum CollisionGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
};

#define COLLIDABLE_MASK	(1<<GROUP_COLLIDABLE_NON_PUSHABLE) | (1<<GROUP_COLLIDABLE_PUSHABLE)

#define MAX_NB_PTS	100
static NxU32 gNbPts = 0;
static NxVec3 gPts[MAX_NB_PTS];

class ControllerHitReport : public NxUserControllerHitReport
{
public:

	void RegisterCamera(Camera *c)
	{
		camera = c;
	}
	
	virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit)
	{
		if(1 && hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			NxActor& actor = hit.shape->getActor();

			if(group == GROUP_COLLIDABLE_PUSHABLE)
			{
				if(actor.isDynamic())
				{
					/*if ((gPts[gNbPts].x != hit.worldPos.x) || (gPts[gNbPts].y != hit.worldPos.y) || (gPts[gNbPts].z != hit.worldPos.z))
					{
						gPts[gNbPts++].x = hit.worldPos.x;
						gPts[gNbPts++].y = hit.worldPos.y;
						gPts[gNbPts++].z = hit.worldPos.z;
						if (gNbPts==MAX_NB_PTS) gNbPts = 0;
					}*/

					// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
					// useless stress on the solver. It would be possible to enable/disable vertical pushes on
					// particular objects, if the gameplay requires it.
					if(hit.dir.y == 0.0f)
					{
						NxF32 coeff = actor.getMass() * hit.length * 5.0f;
						actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
						//NxVec3 p = actor.getGlobalPosition();
						//p.y += 20;
						//actor.setGlobalPosition(p);
						//actor.addForceAtPos(hit.dir*coeff, hit.controller->getPosition(), NX_IMPULSE);
						//actor.addForceAtPos(hit.dir*coeff, hit.worldPos, NX_IMPULSE);
					}
				}
			}
			else if (group == GROUP_COLLIDABLE_NON_PUSHABLE)
			{
				Entity *entity = (Entity*)actor.userData;

				if (entity) // Make sure that we even hit an entity (and not just some static wall)
				{
					if ( entity->enttype == EntityManager::FUNC_BOBBING )
					{
						const Vector3 deltaPos = entity->GetDeltaPos();
						camera->SetFrameShift(deltaPos.x, deltaPos.y, deltaPos.z);
					}
				}
				else
				{
					camera->SetFrameShift(0, 0, 0);
				}
			}
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction onControllerHit(const NxControllersHit& hit)
	{
		return NX_ACTION_NONE;
	}

private:

	Camera *camera;

};

class PhysicsManager
{
public:

	PhysicsManager();

	void Initialize();
	void Deinitialize();

	void Update(const float milliseconds);

	// Physics SDK globals
	NxPhysicsSDK * m_PhysicsSDK;
	NxScene * m_Scene;
	NxControllerManager *controllerManager;
	NxVec3 m_DefaultGravity;

	NxUserAllocatorDefault* myAllocator;

	NxReal m_DeltaTime;
	NxReal m_TotalTime;

	void InitNx();
	void ReleaseNx();
	void ResetNx();

	float GetDeltaTime();
	float GetTotalTime();

	NxActor * CreateBox(NxVec3 * size, bool isStatic);
	NxActor * CreateGroundPlane();
	NxActor * CreateTriangleMesh(int nVertices, int nTris, NxVec3 *verts, NxU16 *tris);
	NxController * CreateCharacterController(Camera *camera, float x, float y, float z);

private:

};