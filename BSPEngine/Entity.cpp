#include "Entity.h"
#include "EntityManager.h"

#include "freeglut.h"

#include <iostream>

Entity::Entity()
{
	numtargets = 0;
}

Entity::~Entity()
{
}

void Entity::SetModel(tModel *model, int lod)
{
	if (!model || lod < 0 || lod > num_lods)
	{
		return;
	}

	models[lod] = model;
}

void Entity::AddTarget(Entity *target_ent)
{
	if (!target_ent) return;

	// When we run out of targets this is going to just crash!

	targets[numtargets] = target_ent;
	numtargets++;
}

void Entity::UpdateTransformMatrix()
{
	if (actor == NULL)
		return;

	NxMat34 pose = actor->getGlobalPose();

	pose.getColumnMajor44(transformation);
}

void Entity::UpdateFuncRotating(float dt)
{
	float t = dt * speed;

	if (spawnflags & 4)
	{
		rotation.x = t;
		rotation.y = 0;
		rotation.z = 0;
	}
	else if (spawnflags & 8)
	{
		rotation.x = 0;
		rotation.y = t;
		rotation.z = 0;
	}
	else
	{
		rotation.x = 0;
		rotation.y = 0;
		rotation.z = t;
	}

	translation = origin; // Just in case we eff'd it somewhere

	NxQuat rot;
	rot.fromAngleAxis(rotation.x, NxVec3(1,0,0));
	actor->setGlobalOrientation(rot);

	UpdateTransformMatrix();
}

void Entity::UpdateFuncBobbing(float dt)
{
	previousTranslation = translation;

	const float twopi = 6.283185;

	float t = dt;
	if (!speed) speed = 5;
	t = t / speed + phase;
	t -= floorf(t);
	t = sinf(twopi * t) * height;
	switch (spawnflags)
	{
		case 1:
			translation.x = -t;
			translation.y = 0;
			translation.z = 0;
		break;
		case 2:
			translation.x = 0;
			translation.y = t;
			translation.z = 0;
		break;
		default:
			translation.x = 0;
			translation.y = 0;
			translation.z = t;
		break;
	}
	
	translation = origin + translation;
	if (actor != NULL)
	{
		actor->setGlobalPosition(NxVec3(translation.x, translation.y, translation.z));
		NxMat33 identity;
		identity.id();
		actor->setGlobalOrientation(identity);
	}

	UpdateTransformMatrix();
}

void Entity::UpdateFuncStatic(float dt)
{
	UpdateTransformMatrix();
}

Vector3 Entity::GetDeltaPos()
{
	return translation - previousTranslation;
}

void Entity::Update(float dt)
{
	switch (enttype)
	{
		case EntityManager::FUNC_BOBBING:
			UpdateFuncBobbing(dt);
		break;
		case EntityManager::FUNC_ROTATING:
			UpdateFuncRotating(dt);
		break;
		case EntityManager::FUNC_STATIC:
			UpdateFuncStatic(dt);
		break;
		/*case ET_FUNC_PENDULUM:
			UpdateFuncPendulum();
		break;
		case ET_FUNC_TRAIN:
			UpdateFuncTrain();
		break;
		case ET_FUNC_TIMER:
			UpdateFuncTimer();
		break;
		case ET_FUNC_DOOR:
			UpdateFuncDoor();
		break;
		case ET_TRIGGER_PUSH:
			UpdateTriggerPush();
		break;
		case ET_TRIGGER_TELEPORT:
			UpdateTriggerTeleport();
		break;
		case ET_TRIGGER_MULTIPLE:
			UpdateTriggerMultiple();
		break;
		case ET_ITEM:
			UpdateItem();
		break;*/
		default:
		break;
	}
}
