#include "PhysicsManager.h"
#include "UpdateTime.h"
#include "Nxc.h"
#include "NxCooking.h"
#include "Stream.h"
#include "../Camera.h"

// <cite> NVIDIA PhysX SDK Documention and Example Projects </cite>

PhysicsManager::PhysicsManager()
{	
	m_TotalTime = 0;
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

	myAllocator = new NxUserAllocatorDefault();
	controllerManager = NxCreateControllerManager(myAllocator);
}

void PhysicsManager::ReleaseNx()
{
	if (m_Scene) m_PhysicsSDK->releaseScene(*m_Scene);
	if (m_PhysicsSDK)  m_PhysicsSDK->release();
}

void PhysicsManager::Update(const float milliseconds)
{
	if (controllerManager != NULL)
	{
		controllerManager->updateControllers();
	}

	if (m_Scene != NULL)
	{
		m_DeltaTime = milliseconds;

		m_TotalTime += m_DeltaTime;

		// Start collision and dynamics for delta time since the last frame
		m_Scene->simulate(m_DeltaTime);
		m_Scene->flushStream();

		while (!m_Scene->fetchResults(NX_RIGID_BODY_FINISHED, false));//m_Scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}
}

float PhysicsManager::GetDeltaTime()
{
	return m_DeltaTime;
}

float PhysicsManager::GetTotalTime()
{
	return m_TotalTime;
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
	if (isStatic)
	{
		boxDesc.group = GROUP_COLLIDABLE_PUSHABLE;
	}
	else
	{
		boxDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;
	}
	actorDesc.shapes.pushBack(&boxDesc);
	
	// Static means immovable
	if (isStatic)
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
		//pActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
	}

	// //create actor with no shapes
	//NxShape* const *shape = pActor->getShapes();
	//NxBoxShape *boxShape = shape[0]->isBox();
	//assert(boxShape);
	//pActor->releaseShape(*boxShape);

	return pActor;
}

void SetActorCollisionGroup(NxActor* actor, NxCollisionGroup group)
{
	NxShape*const* shapes = actor->getShapes();
	NxU32 nShapes = actor->getNbShapes();
	while (nShapes--)
	{
		shapes[nShapes]->setGroup(group);
	}
}

NxActor * PhysicsManager::CreateTriangleMesh(int nVertices, int nTris, NxVec3 *verts, NxU16 *tris)
{
	NxTriangleMeshDesc * triangleMeshDesc = NULL;

    if (!triangleMeshDesc)
	{
		triangleMeshDesc = new NxTriangleMeshDesc();
		assert(triangleMeshDesc);
	}

	triangleMeshDesc->numVertices			= nVertices;
	triangleMeshDesc->pointStrideBytes		= sizeof(NxVec3);
	triangleMeshDesc->points				= verts;
	triangleMeshDesc->numTriangles			= nTris;
	triangleMeshDesc->flags					= NX_MF_16_BIT_INDICES;
	triangleMeshDesc->triangles				= tris;
	triangleMeshDesc->triangleStrideBytes	= 3 * sizeof(NxU16);

	bool isTriMeshDescValid = triangleMeshDesc->isValid();

	//cout << "mesh description validity: " << isTriMeshDescValid << "\n";

	//NxCookingInterface *gCooking=0;
	//gCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);

	//cout << gCooking << "\n";
	
	NxInitCooking();
	MemoryWriteBuffer buf;

	if (triangleMeshDesc == NULL)
		cout << "mesh description error\n";

	bool status = NxCookTriangleMesh(*triangleMeshDesc, buf);
	
	NxTriangleMesh* pMesh;

	if (status)
	{
		pMesh = m_PhysicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
	}
	else
	{
		assert(false);
		pMesh = NULL;
	}
	NxCloseCooking();

	NxTriangleMeshShapeDesc tmsd;
	tmsd.meshData		= pMesh;
	tmsd.userData		= triangleMeshDesc;
	tmsd.localPose.t	= NxVec3(0, 0, 0);
	tmsd.meshPagingMode = NX_MESH_PAGING_AUTO;
	tmsd.group = GROUP_COLLIDABLE_NON_PUSHABLE;
	
	NxActorDesc actorDesc;
	NxBodyDesc  bodyDesc;
	
	assert(tmsd.isValid());
	actorDesc.shapes.pushBack(&tmsd);
	//Dynamic triangle mesh don't be supported anymore. So body = NULL
	//actorDesc.body			= NULL;		
	actorDesc.globalPose.t	= NxVec3(0.0f, 0.0f, 0.0f);

	if (pMesh)
	{
		// Save mesh in userData for drawing
		pMesh->saveToDesc(*triangleMeshDesc);
		//
		assert(actorDesc.isValid());
		NxActor *actor = m_Scene->createActor(actorDesc);
		assert(actor);

		return actor;
	}

	return NULL;
}

NxController * PhysicsManager::CreateCharacterController(Camera *camera, float x, float y, float z)
{
	NxCapsuleControllerDesc desc;
	desc.position.x		= x;
	desc.position.y		= y;
	desc.position.z		= z;
	desc.radius			= 8;
	desc.height			= 52;
	desc.upDirection	= NX_Y;
	desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	desc.slopeLimit		= 0;
	desc.skinWidth		= 0.01;
	desc.stepOffset		= 0.0;
	desc.stepOffset		= 2;
	//	desc.stepOffset	= 0.01f;
	//	desc.stepOffset		= 0;	// Fixes some issues
	//	desc.stepOffset		= 10;
	desc.callback		= camera->GetHitCallback();

	return controllerManager->createController(m_Scene, desc);
}