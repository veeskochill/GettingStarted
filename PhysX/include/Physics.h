#include <PxPhysicsAPI.h>
#include <PxExtensionsAPI.h>

class Renderer;

using namespace physx;

class AxActor : PxRigidActor
{

};
class Physics
{
public:
    Physics();
    ~Physics();
    void Shutdown();

    PxScene*        CreateScene(PxVec3 gravity = PxVec3(0,-9.8,0));
    PxRigidActor*   CreateActor(bool isStatic = false);
//    PxShape*        createShape();
    //void MountMaterial(PxMaterial** ppMat, PxU32 uiMatCount);
    PxShape*    CreateBox(PxRigidActor* actor, PxVec3 halfDim = PxVec3(1,1,1));
    PxShape*    CreateSphere(PxRigidActor* actor, PxReal radius = 1);
    PxShape*    CreatePlane(PxRigidActor* actor);

    void Render(Renderer* pRenderer);

    PxMaterial* CreateMaterial(PxShape* shape, PxReal sFriction, PxReal dFriction, PxReal restitution);

    static Physics* GetManager();
    PxPhysics* GetSDK();
    PxU32 StepPhysX(PxReal timestep);

void DrawShape(PxShape* shape, Renderer* pRenderer);
void DrawActor(PxRigidActor* actor, Renderer* pRenderer);
void RenderActors(Renderer* pRenderer);



private:
    static Physics* gPhysics;
    static PxPhysics* pPhysicsSDK;
    PxMaterial** ppMaterials;
    PxU32 uiMaterialCount;

    PxMaterial* pDefaultMaterial;

};
