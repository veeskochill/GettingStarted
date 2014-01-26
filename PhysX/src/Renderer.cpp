#include <Renderer.h>

Renderer* Renderer::gRenderer = NULL;

Renderer::Renderer(int argc, char** argv):WINDOW_SIZE(1024)
{
    glutInit(&argc, argv);
    InitGL();
}
Renderer::~Renderer()
{}

void Renderer::Shutdown()
{}

void Renderer::InitGL()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);

    glutCreateWindow ("hello");

  /* Enable a single OpenGL light. */
    GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {0,0,0,1.0};  /* Infinite light location. */

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 30.0);
  glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -15.0);
  glRotatef(30, 1.0, 0.0, 0.0);
  glRotatef(-30, 0.0, 1.0, 0.0);
}

Renderer* Renderer::GetManager(int argc, char** argv)
{
    if(!gRenderer)
    {
        gRenderer = new Renderer(argc, argv);
    }
    return gRenderer;
}

void Renderer::DrawBox(float sideLength, float matrix[] )
{
    glPushMatrix(); 
        glMultMatrixf(matrix);
        glutSolidCube(sideLength);
    glPopMatrix(); 
}

void Renderer::PreDraw()
{
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.5,0.5,0.5);
}
void Renderer::PostDraw()
{
      glutSwapBuffers();
}
void Renderer::DrawSphere(float radius, float matrix[]) 
{
    glColor3f(0,1,0);
    glPushMatrix(); 
        glMultMatrixf(matrix);
        glutSolidSphere(radius,10,10);
    glPopMatrix(); 
}
