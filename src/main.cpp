#include<iostream>
#include<GL/glut.h>
#include"fluid.h"

void Render();

struct GraphicObject
{
    unsigned int n_grid_vertices;
    unsigned int n_grid_indices; 
    float* grid_vertex_array;
    unsigned int* grid_index_array;
    Fluid* source;

    GraphicObject(Fluid& f);
    ~GraphicObject();
};

GraphicObject* gl_input;

int main(int argc, char **argv) 
{
    Fluid f(10, 10, 10, 0.001);

    f.InitialiseVortex();
    // // f.PrintFluid();
    // // for (int i = 0; i < 50000; ++i)
    // // {
    // //     f.FiniteDifference();
    // // }

    // // f.PrintFluid();

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

    // enter the processing loop
    glutMainLoop();

    std::cout << "hellow fluid" << std::endl;

    return 0;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float*& VAO = gl_input->grid_vertex_array;

    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < gl_input->n_grid_indices; ++i)
    {   
        unsigned int vertex_ptr = gl_input->grid_index_array[i] * 7;

        // perspective transform
        float n = -1.0f;
        float f = -2.0f;
        float w = -VAO[vertex_ptr + 2];

        float x = (VAO[vertex_ptr] * n) / w;
        float y = (VAO[vertex_ptr + 1] * n) / w;
        float z = ((VAO[vertex_ptr + 2] * (-(f + n) / (f - n))) - (2*n*f / (f - n))) / w;

        glColor4f(VAO[vertex_ptr + 3], VAO[vertex_ptr + 4], VAO[vertex_ptr + 5], VAO[vertex_ptr + 6]);
        glVertex3f(x, y, z);
    }
    glEnd();
    glutSwapBuffers();
}

GraphicObject::GraphicObject(Fluid& F)
:
source(&F)
{
    unsigned int& lx = source->x_size;
    unsigned int& ly = source->y_size;
    unsigned int& lz = source->z_size;

    float dx = 2/((float)lx + 1);
    float dy = 2/((float)ly + 1);
    float dz = 2/((float)lz + 1);

    n_grid_vertices = lx*ly*lz;
    n_grid_indices = 2*(3*lx*ly*lz - lx*ly - ly*lz - lx*lz);

    grid_vertex_array = new float[n_grid_vertices * 7];
    grid_index_array = new unsigned int[n_grid_indices * 2];

    unsigned int index_iterator = 0;

    for (unsigned int k = 0; k < lz; ++k)
    {
        for (unsigned int j = 0; j < ly; ++j)
        {
            for (unsigned int i = 0; i < lx; ++i)
            {
                unsigned int iterator = 7*(((ly*k) + j)*lx + i);

                grid_vertex_array[iterator] = ((float)i + 1.0f)*dx - 1.0f;
                grid_vertex_array[iterator + 1] = ((float)j + 1.0f)*dy - 1.0f;
                grid_vertex_array[iterator + 2] = ((float)k + 1.0f)*dz + 1.0f;
                grid_vertex_array[iterator + 3] = 1.0f;
                grid_vertex_array[iterator + 4] = 0.0f;
                grid_vertex_array[iterator + 5] = 1.0f;
                grid_vertex_array[iterator + 6] = 0.2f;

                if (i < lx - 1)
                {
                    grid_index_array[index_iterator] = iterator/7;
                    grid_index_array[index_iterator + 1] = iterator/7 + 1;
                    index_iterator += 2;
                }

                if (j < ly - 1)
                {
                    grid_index_array[index_iterator] = iterator/7;
                    grid_index_array[index_iterator + 1] = iterator/7 + lx;
                    index_iterator += 2;
                }
                
                if (k < lz - 1)
                {
                    grid_index_array[index_iterator] = iterator/7;
                    grid_index_array[index_iterator + 1] = iterator/7 + lx*ly;
                    index_iterator += 2;
                }
            }
        }
    }
}

GraphicObject::~GraphicObject()
{
    delete grid_vertex_array;
    delete grid_index_array;
}