#include<iostream>
#include<GL/glut.h>
#include"graphicobject.h"
#include"fluid.h"

void Render();

void Timer(int state);

GraphicObject* gl_input;

int main(int argc, char **argv) 
{
    Fluid f(10, 10, 10, 0.001);

    f.InitialiseVortex();

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

    // render arrows

    unsigned int n_arrows = gl_input->source->x_size*gl_input->source->y_size*gl_input->source->z_size;
    VAO = gl_input->arrow_vertex_array;

    glLineWidth(3.0);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < n_arrows; ++i)
    {   
        // arrowheads
        float xh = VAO[i*7] + 1.5*VAO[i*7 + 3];
        float yh = VAO[i*7 + 1] + 1.5*VAO[i*7 + 4];
        float zh = VAO[i*7 + 2] + 1.5*VAO[i*7 + 5];

        // arrow tails
        float xt = VAO[i*7];
        float yt = VAO[i*7 + 1];
        float zt = VAO[i*7 + 2];

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

        glColor4f(1.0f - VAO[i*7 + 6], 0.0f + VAO[i*7 + 6], 1.0f - VAO[i*7 + 6], VAO[i*7 + 6]);
        glVertex3f(xt, yt, zt);
        glColor4f(0.0f + VAO[i*7 + 6], 1.0f - VAO[i*7 + 6], 1.0f - VAO[i*7 + 6], VAO[i*7 + 6]);
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
    glutTimerFunc(15, Timer, state);
}