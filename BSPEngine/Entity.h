#pragma once

#include "types.h"
#include "Main.h"
#include "NxPhysics.h"

class Entity
{
public:
	Entity();
	~Entity();

	int model;
    tModel *models[10];		// pointer on associated model, ten is arbitrary (just assuming I might need this many for the LOD)

	NxActor *actor;

	int enttype;
    int itemtype;
    int spawnflags;

	Vector3 origin;

    char*     dst_target;       // name of destination target
    char*     my_target;        // name of current entity as target

	int numchildren;
	int num_lods;
    Entity **children;		// pointer on child entities
    Entity *parent;			// pointer on parent entity
    Entity *targets[10];	// pointer on target entities
    int numtargets;			// number of targets

	float     angle;
    float     wait;
	float     height;
    float     phase;
    float     speed;

	float transformation[16];

	Vector3 GetDeltaPos();

	void Update(float dt);

	void SetModel(tModel *model, int lod);
	void AddTarget(Entity *target_ent);

private:

	Vector3 translation;
	Vector3 rotation;

	Vector3 previousTranslation;

	void UpdateFuncRotating(float dt);
	void UpdateFuncBobbing(float dt);
	void UpdateFuncStatic(float dt);

	void UpdateTransformMatrix();

};
