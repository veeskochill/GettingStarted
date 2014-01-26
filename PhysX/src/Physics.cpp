#include "Physics.h"
#include <Renderer.h>
using namespace physx;

Physics* Physics::gPhysics = NULL;
PxPhysics* Physics::pPhysicsSDK = NULL;
Physics::Physics():ppMaterials(NULL),uiMaterialCount(0)
{
    PxFoundation* gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,
    *(new PxDefaultAllocator()), *( new PxDefaultErrorCallback()));
   // PxTolerancesScale tolerance;

    pPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
  //  if(!pPhysicsSDK)
  //      return false;

    pDefaultMaterial = pPhysicsSDK->createMaterial(0.0,0.0,1.0);
    //return true;
}

Physics::~Physics()
{
    if(pPhysicsSDK)
        Shutdown();
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
        ppScenes[ui]->release();
    }
    pPhysicsSDK->release();
}

Physics* Physics::GetManager()
{
    if(!gPhysics)
    {
        gPhysics = new Physics();
    }
    return gPhysics;
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

PxShape* Physics::CreatePlane(PxRigidActor* actor)
{
    PxPlaneGeometry planeGeom = PxPlaneGeometry();
    return actor->createShape(planeGeom, *pDefaultMaterial);
    if(actor->isRigidBody())
        PxRigidBodyExt::updateMassAndInertia(*actor->isRigidBody(),1,&PxVec3(0,0,0));
}

PxShape* Physics::CreateBox(PxRigidActor* actor, PxVec3 halfDim)
{
    PxBoxGeometry boxGeom = PxBoxGeometry(halfDim);
    return actor->createShape(boxGeom, *pDefaultMaterial);
    if(actor->isRigidBody())
        PxRigidBodyExt::updateMassAndInertia(*actor->isRigidBody(),1,&PxVec3(0,0,0));
}

PxShape* Physics::CreateSphere(PxRigidActor* actor, PxReal radius)
{
    PxSphereGeometry sphereGeom = PxSphereGeometry(radius);
    return actor->createShape(sphereGeom, *pDefaultMaterial);
    if(actor->isRigidBody())
        PxRigidBodyExt::updateMassAndInertia(*actor->isRigidBody(),1,&PxVec3(0,0,0));
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
        if(errorState)
            return errorState;
    }
    return errorState;
}


void getColumnMajor(PxMat33 m, PxVec3 t, float* mat) {
   mat[0] = m.column0[0];
   mat[1] = m.column0[1];
   mat[2] = m.column0[2];
   mat[3] = 0;

   mat[4] = m.column1[0];
   mat[5] = m.column1[1];
   mat[6] = m.column1[2];
   mat[7] = 0;

   mat[8] = m.column2[0];
   mat[9] = m.column2[1];
   mat[10] = m.column2[2];
   mat[11] = 0;

   mat[12] = t[0];
   mat[13] = t[1];
   mat[14] = t[2];
   mat[15] = 1;
}

void Physics::DrawShape(PxShape* shape, Renderer* pRenderer) 
{ 
    PxGeometryType::Enum type = shape->getGeometryType();
    PxTransform pT = PxShapeExt::getGlobalPose(*shape);

    PxMat33 m = PxMat33(pT.q);
    float mat[16];
    getColumnMajor(m,pT.p, mat);

    switch(type) 
    {          
        case PxGeometryType::eBOX:
        {
            PxBoxGeometry bg;
            shape->getBoxGeometry(bg);
            pRenderer->DrawBox(2*bg.halfExtents.x,mat);
            break;
        }
        case PxGeometryType::eSPHERE :
        {
            PxSphereGeometry sg;
            shape->getSphereGeometry(sg);
            pRenderer->DrawSphere(sg.radius, mat);
            break;
        }
    } 
} 

void Physics::DrawActor(PxRigidActor* actor, Renderer* pRenderer) 
{  
    PxTransform actorPose = actor->getGlobalPose();
    PxU32 nShapes = actor->getNbShapes(); 
    PxShape** shapes=new PxShape*[nShapes];
    
    actor->getShapes(shapes, nShapes);     
    while (nShapes--) 
    { 
        DrawShape(shapes[nShapes], pRenderer); 
    } 
    delete [] shapes;
} 

void Physics::RenderActors(Renderer* pRenderer)
{ 
    // Render all the actors in the scene 
    PxU32 uiSceneCount = gPhysics->GetSDK()->getNbScenes();
    PxScene** ppScenes = (PxScene**)malloc(uiSceneCount*sizeof(PxScene*));
    gPhysics->GetSDK()->getScenes(ppScenes, uiSceneCount);
    for(PxU32 ui=0;ui<uiSceneCount;ui++)
    {
        PxU32 uiActorCount = ppScenes[ui]->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC |PxActorTypeSelectionFlag::eRIGID_STATIC);
        PxRigidActor** ppActors = (PxRigidActor**)malloc(uiActorCount*sizeof(PxRigidActor*));
        ppScenes[ui]->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC |PxActorTypeSelectionFlag::eRIGID_STATIC, (PxActor**)ppActors, uiActorCount);
        for(PxU32 uj=0;uj<uiActorCount;uj++)
        {
            DrawActor(ppActors[uj], pRenderer);
        }
    }
}

void Physics::Render(Renderer* pRenderer)
{
    pRenderer->PreDraw();
    RenderActors(pRenderer);
    pRenderer->PostDraw();
}
