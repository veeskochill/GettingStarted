#include <PxPhysicsAPI.h>
#include <PxExtensionsAPI.h>

using namespace physx;

class Physics
{
public:
    Physics();
    ~Physics();
    bool Init(PxErrorCallback& error, PxDefaultAllocator& alloc);
    void Shutdown();

    PxScene*        CreateScene(PxVec3 gravity = PxVec3(0,-9.8,0));
    PxRigidActor*   CreateActor(bool isStatic = false);
//    PxShape*        createShape();
    //void MountMaterial(PxMaterial** ppMat, PxU32 uiMatCount);
    PxShape*    CreateBox(PxRigidActor* actor, PxVec3 halfDim = PxVec3(1,1,1));
    PxShape*    CreateSphere(PxRigidActor* actor, PxReal radius = 1);
    PxShape*    CreatePlane(PxRigidActor* actor);

    PxMaterial* CreateMaterial(PxShape* shape, PxReal sFriction, PxReal dFriction, PxReal restitution);

    PxPhysics* GetSDK();
    PxU32 StepPhysX(PxReal timestep);
   // PxErrorCallback& getSampleErrorCallback();
  //  PxDefaultAllocator& getSampleAllocatorCallback();

    
private:
    PxPhysics* pPhysicsSDK;
    PxMaterial** ppMaterials;
    PxU32 uiMaterialCount;

    PxMaterial* pDefaultMaterial;
//    static PxDefaultErrorCallback gDefaultErrorCallback;
//    static PxDefaultAllocator gDefaultAllocatorCallback;//("APEX", NULL,false);

};
