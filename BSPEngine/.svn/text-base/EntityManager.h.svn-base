#pragma once

#include "Entity.h"
#include <map>
#include <vector>

using namespace std;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	static const int FUNC_ROTATING = 10;
	static const int FUNC_BOBBING = 11;
	static const int FUNC_STATIC = 666;
	static const int TRIGGER_HURT = 42;

	int numEntities;
	Entity entities[100]; // Just an arbitrary number of entities I might need

	void ProcessTriggerHurt(int i, map<string, string> entityProps);
	void ProcessFuncBobbing(int i, map<string, string> entityProps);
	void ProcessFuncRotating(int i, map<string, string> entityProps);
	void ProcessFuncStatic(int i, map<string, string> entityProps);
	void ProcessEntities(const char * entityString);
};
