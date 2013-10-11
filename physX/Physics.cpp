#include "Physics.h"

using namespace physx;
//PxDefaultAllocator gDefaultAllocatorCallback;// = PxDefaultAllocator;//("APEX", NULL,false);


Physics::Physics():pPhysicsSDK(NULL),ppMaterials(NULL),uiMaterialCount(0)
{

    //gDefaultErrorCallback = PxDefaultErrorCallback();
    //gDefaultAllocatorCallback = PxDefaultAllocator();//("APEX", NULL,false);
}
Physics::~Physics()
{
    if(pPhysicsSDK)
        Shutdown();
}
bool Physics::Init(PxErrorCallback& error, PxDefaultAllocator& alloc)
{
    if(pPhysicsSDK)
    {
        return true;
    }
    else
    {
//PxDefaultErrorCallback gDefaultErrorCallback;// = PxDefaultErrorCallback;

       // PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
        PxFoundation* gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,alloc, error);
        
        pPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
        if(!pPhysicsSDK)
            return false;
    }

    pDefaultMaterial = pPhysicsSDK->createMaterial(0.5,0.5,0.5);
    return true;
}


void Physics::Shutdown()
{
    PxActorTypeSelectionFlags flags;
    flags |= PxActorTypeSelectionFlag::eRIGID_DYNAMIC| PxActorTypeSelectionFlag::eRIGID_STATIC;
    
    PxU32 uiSceneCount = pPhysicsSDK->getNbScenes();
    PxScene** ppScenes = (PxScene**)malloc(uiSceneCount*sizeof(PxScene*));
    pPhysicsSDK->getScenes(ppScenes, uiSceneCount);
    
    for(PxU32 ui =0;ui<uiSceneCount; ui++)
    {
  /*      PxU32 uiActorCount = ppScenes[ui]->getNbActors(
            PxActorTypeSelectionFlag::eRIGID_DYNAMIC | 
            PxActorTypeSelectionFlag::eRIGID_STATIC);
        PxActor** ppActors = (PxActor**)malloc(uiActorCount*sizeof(PxActor*));
        ppScenes[ui]->getActors(flags,ppActors,uiActorCount);
        
        for(PxU32 uj=0;uj<uiActorCount;uj++)
        {
            ppActors[uj]->release();
        }
        */
        ppScenes[ui]->release();
    }
    pPhysicsSDK->release();
}

PxPhysics* Physics::GetSDK()
{
    return pPhysicsSDK;
}

PxScene* Physics::CreateScene(PxVec3 gravity)
{
    PxSceneDesc sceneDesc(pPhysicsSDK->getTolerancesScale());
    sceneDesc.gravity = gravity;
    if(!sceneDesc.cpuDispatcher) {
        PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(4);
        sceneDesc.cpuDispatcher = mCpuDispatcher;
    }
    if(!sceneDesc.filterShader)
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    if(!sceneDesc.isValid())
        return NULL;
    else
        return pPhysicsSDK->createScene(sceneDesc);
}

PxRigidActor* Physics::CreateActor(bool isStatic)
{
    PxTransform globalPose = PxTransform::createIdentity();
    if(isStatic)
        return (PxRigidActor*)(GetSDK()->createRigidStatic(globalPose));
    else
        return (PxRigidActor*)(GetSDK()->createRigidDynamic(globalPose));
}
/*
void Physics::MountMaterial(PxMaterial** ppMat, PxU32 uiMatCount)
{
    ppMaterials = ppMat;
    uiMaterialCount = uiMatCount;
}
*/
PxShape* Physics::CreatePlane(PxRigidActor* actor)
{
    PxPlaneGeometry planeGeom = PxPlaneGeometry();
    return actor->createShape(planeGeom, *pDefaultMaterial);
    PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)actor,1,&PxVec3(0,0,0));
}

PxShape* Physics::CreateBox(PxRigidActor* actor, PxVec3 halfDim)
{
    PxBoxGeometry boxGeom = PxBoxGeometry(halfDim);
    return actor->createShape(boxGeom, *pDefaultMaterial);
    PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)actor,1,&PxVec3(0,0,0));

}
PxShape* Physics::CreateSphere(PxRigidActor* actor, PxReal radius)
{
    PxSphereGeometry sphereGeom = PxSphereGeometry(radius);
    return actor->createShape(sphereGeom, *pDefaultMaterial);
}

PxMaterial* Physics::CreateMaterial(PxShape* shape, PxReal sFriction, PxReal dFriction, PxReal restitution)
{
    PxMaterial* pMaterial = pPhysicsSDK->createMaterial(sFriction, dFriction, restitution);
    shape->setMaterials(&pMaterial, 1);
    return pMaterial;
}

PxU32 Physics::StepPhysX(PxReal myTimestep) 
{
    PxU32 uiSceneCount = pPhysicsSDK->getNbScenes();
    PxScene** ppScenes = (PxScene**)malloc(uiSceneCount*sizeof(PxScene*));
    pPhysicsSDK->getScenes(ppScenes, uiSceneCount);
    physx::PxU32 errorState = 0;
    for(PxU32 ui =0;ui<uiSceneCount;ui++)
    {
        ppScenes[ui]->simulate(myTimestep);
        ppScenes[ui]->fetchResults(true, &errorState);
//        if(errorState)
//            return errorState;
    }
    return errorState;
}