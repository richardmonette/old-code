#include "PhysicsManager.h"
#include "UpdateTime.h"

PhysicsManager * PhysicsManager::pInstance = NULL;

PhysicsManager * PhysicsManager::GetInstance()
{	
	if (!pInstance)
	{
		pInstance = new PhysicsManager;
	}

	return pInstance;
}

void PhysicsManager::Initialize()
{
	InitNx();

	CreateGroundPlane();
}

void PhysicsManager::Deinitialize()
{
	ReleaseNx();
}

void PhysicsManager::InitNx()
{
	// Create the physics SDK
	m_PhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if (!m_PhysicsSDK)  return;

	// Set the physics parameters
	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);

	// Set the debug visualization parameters
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

	m_DefaultGravity = NxVec3(0, -9.8, 0);

	// Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.simType				= NX_SIMULATION_SW;
	sceneDesc.gravity               = m_DefaultGravity;
	m_Scene = m_PhysicsSDK->createScene(sceneDesc);	
	if(!m_Scene)
	{ 
		sceneDesc.simType			= NX_SIMULATION_SW; 
		m_Scene = m_PhysicsSDK->createScene(sceneDesc);  
		if(!m_Scene) return;
	}

	// Create the default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	// Get the current time
	UpdateTime();

	// Start the first frame of the simulation
	if (m_Scene)  StartPhysics();
}

void PhysicsManager::ReleaseNx()
{
	if (m_Scene)
	{
		GetPhysicsResults();  // Make sure to fetchResults() before shutting down
		m_PhysicsSDK->releaseScene(*m_Scene);
	}
	if (m_PhysicsSDK)  m_PhysicsSDK->release();
}

void PhysicsManager::Update()
{
	if (m_Scene)
	{
		GetPhysicsResults();
		StartPhysics();
	}
}

void PhysicsManager::StartPhysics()
{
	// Update the time step
	m_DeltaTime = UpdateTime();

	// Start collision and dynamics for delta time since the last frame
	m_Scene->simulate(m_DeltaTime);
	m_Scene->flushStream();
}

void PhysicsManager::GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	while (!m_Scene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}

NxActor* PhysicsManager::CreateGroundPlane()
{
	// Create a plane with default descriptor
	NxPlaneShapeDesc planeDesc;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	return m_Scene->createActor(actorDesc);
}

NxActor * PhysicsManager::CreateBox(NxVec3 * size, bool isStatic)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(size->x, size->y, size->z);
	boxDesc.localPose.t = NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&boxDesc);
	
	// Static means immovable
	if (!isStatic)
	{
		actorDesc.body = &bodyDesc;
	}

	actorDesc.density = 10.0f;
	actorDesc.globalPose.t = NxVec3(0, 0 ,0);	
	assert(actorDesc.isValid());
	NxActor *pActor = m_Scene->createActor(actorDesc);	
	assert(pActor);

	if (isStatic)
	{
		pActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
	}

	// //create actor with no shapes
	//NxShape* const *shape = pActor->getShapes();
	//NxBoxShape *boxShape = shape[0]->isBox();
	//assert(boxShape);
	//pActor->releaseShape(*boxShape);

	return pActor;
}