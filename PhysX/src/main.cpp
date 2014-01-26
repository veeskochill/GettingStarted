#include <fstream>
#include <iostream>
#include <stdio.h>
#include "C:/freeglut-2.6.0/include/GL/freeglut.h"

#include <Physics.h>
#include <Renderer.h>

using namespace std;
using namespace physx;

Physics* gPhysics = NULL;
Renderer* gRenderer = NULL;

PxReal myTimestep = 1.0f/60.0f;

void OnShutdown() {
    gRenderer->Shutdown();
    gPhysics->Shutdown();
}

void mainLoop()
{
    bool bCloseRequested = false;

    int deltaTime = 0;
    int stepSize = 1000/30;

    while (!bCloseRequested)
    {
        int iterTime = 0;
        int oldTime = glutGet(GLUT_ELAPSED_TIME);

        while( deltaTime >= stepSize)
        {
            //Step State (stepSize)
            gPhysics->StepPhysX((float)stepSize/2000.0f);
            deltaTime -= stepSize;
            iterTime += stepSize;
        }

         gPhysics->Render(gRenderer);

        deltaTime += glutGet(GLUT_ELAPSED_TIME) - oldTime;
    }
}

void main(int argc, char** argv) {
    atexit(OnShutdown);
 
    gRenderer = Renderer::GetManager(argc, argv);
    gPhysics = Physics::GetManager();

    PxRigidActor** ppActors = (PxRigidActor**)malloc(2*sizeof(PxRigidActor*));

    ppActors[0] = gPhysics->CreateActor(true);
    ppActors[1] = gPhysics->CreateActor();

    //gPhysics->CreatePlane(ppActors[0]);
    gPhysics->CreateBox(ppActors[0]);
    gPhysics->CreateBox(ppActors[1]);

  //  ppActors[0]->isRigidActor()->setGlobalPose(PxTransform(PxQuat(PxHalfPi,PxVec3(0,0,1))));
    ppActors[0]->isRigidActor()->setGlobalPose(
        PxTransform(
            PxMat44(
                PxMat33(PxVec3(0,1,0),PxVec3(-1,0,0),PxVec3(0,0,1)),
                PxVec3(0,0,0)
            )
        )
    ); //rotate about z by 90
    ppActors[1]->isRigidActor()->setGlobalPose(PxTransform(PxVec3(0,5,0)));

    gPhysics->CreateScene()->addActors(2, (PxActor**)ppActors);
    mainLoop();
}
