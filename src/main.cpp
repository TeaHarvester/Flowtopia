#include<iostream>
#include<GL/glut.h>
#include"graphicobject.h"
#include"fluid.h"
#define pi 3.1415926535f

void Render();
void Timer(int state);
void ProcessNormalKeys(unsigned char key, int x, int y);

GraphicObject* gl_input;

int main(int argc, char **argv) 
{
    Fluid f(19, 19, 19, 0.001);
    f.object = new RigidBody(Vector3<double>(5, 9, 9), 4);
    f.InitialiseFlow(Vector3<double>(1, 0, 0));
 
    GraphicObject g(f);
    gl_input = &g;


    // initialise freeglut and open window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Flowtopia");

    // enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // register callbacks
    glutKeyboardFunc(ProcessNormalKeys);
    glutDisplayFunc(Render);
    glutTimerFunc(15, Timer, 0);

    // enter the processing loop
    glutMainLoop();
    
    std::cout << "hellow fluid" << std::endl;

    return 0;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float* VAO = gl_input->grid_vertex_array;

    gl_input->OrientGrid(gl_input->orientation);
    gl_input->OrientArrows();

    // render grid
    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < gl_input->n_grid_indices; ++i)
    {   
        unsigned int vertex_ptr = gl_input->grid_index_array[i] * 7;

        // vertex shader
        float n = -1.0f;
        float f = -10.0f;

        float w = -VAO[vertex_ptr + 2];
        float x = (VAO[vertex_ptr] * n) / w;
        float y = (VAO[vertex_ptr + 1] * n) / w;
        float z = ((VAO[vertex_ptr + 2] * (-(f + n) / (f - n))) - (2*n*f / (f - n))) / w;

        glColor4f(VAO[vertex_ptr + 3], VAO[vertex_ptr + 4], VAO[vertex_ptr + 5], VAO[vertex_ptr + 6]);
        glVertex3f(x, y, z);
    }
    glEnd();

    gl_input->OrientGrid(gl_input->orientation.Conj());

    // render arrows

    unsigned int n_arrows = gl_input->n_arrow_vertices;
    VAO = gl_input->arrow_vertex_array;

    glLineWidth(3.0);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < n_arrows; ++i)
    {   
        if (VAO[i*14 + 6] == 0.0f)
        {
            continue;
        }

        // arrow tails
        float xt = VAO[i*14];
        float yt = VAO[i*14 + 1];
        float zt = VAO[i*14 + 2];

        // arrow heads
        float xh = VAO[i*14 + 7];
        float yh = VAO[i*14 + 8];
        float zh = VAO[i*14 + 9];

        // vertex shader
        float n = -1.0f;
        float f = -2.0f;

        float w = -zt;
        xt *= n/w;
        yt *= n/w;
        zt = ((zt * (-(f + n) / (f - n))) - (2*n*f / (f - n))) / w;

        w = -zh;
        xh *= n / w;
        yh *= n / w;
        zh = ((zh * (-(f + n) / (f - n))) - (2*n*f / (f - n))) / w;

        glColor4f(VAO[i*14 + 3], VAO[i*14 + 4], VAO[i*14 + 5], VAO[i*14 + 5]);
        glVertex3f(xt, yt, zt);
        glColor4f(VAO[i*14 + 10], VAO[i*14 + 11], VAO[i*14 + 12], VAO[i*14 + 13]);
        glVertex3f(xh, yh, zh);
    }
    glEnd();

    glutSwapBuffers();
}

void Timer(int state)
{
    gl_input->source->FiniteDifference();
    gl_input->GetArrowVertexArray();
    glutPostRedisplay();
    glutTimerFunc(5, Timer, state);
}

void ProcessNormalKeys(unsigned char key, int x, int y)
{
    Quaternion<float> rotation(0.0f, 0.0f, 0.0f, 0.0f);
    float angle = pi/24;

    switch(key)
    {
        case 32:
        gl_input->source->FiniteDifference();
        break;

        case 97:
        rotation = Quaternion<float>(cos(angle), 0.0f, sin(angle), 0.0f);
        gl_input->orientation = rotation*gl_input->orientation;
        break;

        case 100:
        rotation = Quaternion<float>(cos(-angle), 0.0f, sin(-angle), 0.0f);
        gl_input->orientation = rotation*gl_input->orientation;
        break;

        case 119:
        rotation = Quaternion<float>(cos(angle), sin(angle), 0.0f, 0.0f);
        gl_input->orientation = rotation*gl_input->orientation;
        break;

        case 115:
        rotation = Quaternion<float>(cos(-angle), sin(-angle), 0.0f, 0.0f);
        gl_input->orientation = rotation*gl_input->orientation;
        break;
    }
}