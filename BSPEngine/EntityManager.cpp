#include "EntityManager.h"

#include <string>

EntityManager::EntityManager()
{
	numEntities = 0;
}

EntityManager::~EntityManager()
{
}

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void EntityManager::ProcessFuncRotating(int i, map<string, string> entityProps)
{
  entities[i].enttype = FUNC_ROTATING;
  entities[i].model = atoi(entityProps["model"].c_str()); // this may need to be +1 for the offset
  entities[i].speed = atof(entityProps["speed"].c_str());

  // default values
  if (!entities[i].speed) entities[i].speed = 50;
}

void EntityManager::ProcessFuncBobbing(int i, map<string, string> entityProps)
{
	entities[i].enttype = FUNC_BOBBING;
	entities[i].model = atoi(entityProps["model"].c_str()); // this may need to be +1 for the offset
	entities[i].height = atof(entityProps["height"].c_str());
	entities[i].phase = atof(entityProps["phase"].c_str());
	entities[i].speed = atof(entityProps["speed"].c_str());

	// Put in defaults if they were not set
	if (!entities[i].speed) entities[i].speed = 8;
	if (!entities[i].phase) entities[i].phase = 0;
	if (!entities[i].height) entities[i].height = 32;
}

void EntityManager::ProcessFuncStatic(int i, map<string, string> entityProps)
{
	entities[i].enttype = FUNC_STATIC;
	entities[i].model = atoi(entityProps["model"].c_str());
}

void EntityManager::ProcessTriggerHurt(int i, map<string, string> entityProps)
{
	entities[i].enttype = TRIGGER_HURT;
	entities[i].model = atoi(entityProps["model"].c_str()); // this may need to be +1 for the offset
}

void EntityManager::ProcessEntities(const char * entityString)
{
	//Break apart the entity string (going to use some STL here in order to make life bit easier)

	vector<string> entityStrings;
	
	string stlentitystring = entityString;

	int position = stlentitystring.find("{");
	while ( position != string::npos ) 
	{
		stlentitystring.replace( position, 1, "|" );
		position = stlentitystring.find( "{", position + 1 );
	} 

	position = stlentitystring.find("}");
	while ( position != string::npos ) 
	{
		stlentitystring.replace( position, 1, "" );
		position = stlentitystring.find( "}", position + 1 );
	} 

	position = stlentitystring.find("*");
	while ( position != string::npos ) 
	{
		stlentitystring.replace( position, 1, "" );
		position = stlentitystring.find( "*", position + 1 );
	} 

	Tokenize(stlentitystring, entityStrings, "|");

	for (int i = 0; i < entityStrings.size(); i++) 
	{
		map<string, string> entityProps;
		vector<string> entityFragments;

		position = entityStrings[i].find("\"");
		while ( position != string::npos ) 
		{
			entityStrings[i].replace( position, 1, "|" );
			position = entityStrings[i].find( "\"", position + 1 );
		}

		position = entityStrings[i].find("\n");
		while ( position != string::npos ) 
		{
			entityStrings[i].replace( position, 1, "" );
			position = entityStrings[i].find( "\n", position + 1 );
		}

		position = entityStrings[i].find("||");
		while ( position != string::npos ) 
		{
			entityStrings[i].replace( position, 2, "|" );
			position = entityStrings[i].find( "||", position + 1 );
		}

		position = entityStrings[i].find("| |");
		while ( position != string::npos ) 
		{
			entityStrings[i].replace( position, 3, "|" );
			position = entityStrings[i].find( "| |", position + 1 );
		}

		Tokenize(entityStrings[i], entityFragments, "|");

		for (int k = 0; k < entityFragments.size()-1; k+=2)
		{
			entityProps[entityFragments[k]] = entityFragments[k+1]; 
		}	

		entities[i].spawnflags = 0;
		entities[i].spawnflags = atoi(entityProps["spawnflags"].c_str());
		
		string originStr = entityProps["origin"];
		if (originStr.size() > 0)
		{
			vector<string> originVector;
			Tokenize(originStr, originVector, " ");
			entities[i].origin = Vector3(atof(originVector[0].c_str()), atof(originVector[1].c_str()), atof(originVector[2].c_str()));

			// Swap the y and z values, and negate the new z so Y is up (getting out of the retarded q3 space)
			float temp = entities[i].origin.y;
			entities[i].origin.y = entities[i].origin.z;
			entities[i].origin.z = -temp;
		}
		
		entities[i].angle = atof(entityProps["angle"].c_str());
		entities[i].my_target = (char*)entityProps["targetname"].c_str();
		entities[i].dst_target = (char*)entityProps["target"].c_str();
		entities[i].wait = atof(entityProps["wait"].c_str());

		const char *classname_val = (char*)entityProps["classname"].c_str();

		if (!stricmp(classname_val, "worldspawn"))
		{
			int i = 0;// Setup where the player goes
		}
		else if (!stricmp(classname_val, "func_bobbing")) 
		{
			ProcessFuncBobbing(i, entityProps);
		}
		else if (!stricmp(classname_val, "func_rotating")) 
		{
			ProcessFuncRotating(i, entityProps);
		}
		else if (!stricmp(classname_val, "trigger_hurt")) 
		{
			ProcessTriggerHurt(i, entityProps);
		}
		else if (!stricmp(classname_val, "func_static")) 
		{
			ProcessFuncStatic(i, entityProps);
		}

		numEntities++;
	}

	/* find the target for each entity
	for (int i = 0; i < numEntities; ++i)
	{
		if (entities[i].dst_target)
		{
			for (int j = 0; j < numEntities; ++j)
			{
				if (!entities[j].my_target) continue;

				if (!stricmp(entities[j].my_target, entities[i].dst_target))
				{
					entities[i].AddTarget(&entities[j]);
				}
			}
		}
	}*/

	int b = 0;
}
