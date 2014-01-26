#include "C:/freeglut-2.6.0/include/GL/freeglut.h"

class Renderer
{
public:
    Renderer(int argc =0, char** argv=NULL);
    ~Renderer();
    static bool Init();
    void Shutdown();

    void PreDraw();
    void PostDraw();

    void DrawBox(float sideLength, float[]);
    void DrawSphere(float radius, float[]);
    void InitGL();

    static Renderer* GetManager(int argc =0, char** argv=NULL);

private:
    static Renderer* gRenderer;

    const int WINDOW_SIZE;
};

