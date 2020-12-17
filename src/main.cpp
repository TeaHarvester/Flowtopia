#include<iostream>
#include<GL/glut.h>
#include"fluid.h"

void Render();

void Timer(int state);

struct GraphicObject
{
    unsigned int n_grid_vertices;
    unsigned int n_grid_indices; 
    float* grid_vertex_array;
    unsigned int* grid_index_array;
    float* arrow_vertex_array;
    Fluid* source;
    void GetArrowVertexArray();
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
        float f = -2.0f;

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

GraphicObject::GraphicObject(Fluid& F)
:
source(&F)
{
    unsigned int lx = source->x_size + 1;
    unsigned int ly = source->y_size + 1;
    unsigned int lz = source->z_size + 1;

    float dx = 2/((float)lx + 1);
    float dy = 2/((float)ly + 1);
    float dz = 2/((float)lz + 1);

    // write grid arrays

    n_grid_vertices = lx*ly*lz;
    n_grid_indices = 2*(3*lx*ly*lz - lx*ly - ly*lz - lx*lz);

    grid_vertex_array = new float[n_grid_vertices * 7];
    grid_index_array = new unsigned int[n_grid_indices * 2];
    arrow_vertex_array = new float[lx*ly*lz*7];

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
                grid_vertex_array[iterator + 3] = 0.8f;
                grid_vertex_array[iterator + 4] = 0.8f;
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

    GetArrowVertexArray();
}

void GraphicObject::GetArrowVertexArray()
{
    // write arrow arrays
    unsigned int lx = source->x_size + 1;
    unsigned int ly = source->y_size + 1;
    unsigned int lz = source->z_size + 1;

    float dx = 2/((float)lx + 1);
    float dy = 2/((float)ly + 1);
    float dz = 2/((float)lz + 1);

    float norm_coeff = (float)source->u->norm_coeff;

    for (unsigned int k = 0; k < lz - 1; ++k)
    {
        for (unsigned int j = 0; j < ly - 1; ++j)
        {
            for (unsigned int i = 0; i < lx - 1; ++i)
            {
                unsigned int iterator = 7*(((ly*k) + j)*lx + i);

                arrow_vertex_array[iterator] = ((float)i + 1.5f)*dx - 1.0f;
                arrow_vertex_array[iterator + 1] = ((float)j + 1.5f)*dy - 1.0f;
                arrow_vertex_array[iterator + 2] = ((float)k + 1.5f)*dz + 1.0f;

                // temporarily store vector field in vertex colour array

                Vector3* flow = (*source->u)(i, j, k);

                arrow_vertex_array[iterator + 3] = dx*(float)(*flow)(0)/norm_coeff;
                arrow_vertex_array[iterator + 4] = dy*(float)(*flow)(1)/norm_coeff;
                arrow_vertex_array[iterator + 5] = dz*(float)(*flow)(2)/norm_coeff;
                arrow_vertex_array[iterator + 6] = (float)flow->Magnitude()/norm_coeff;
            }
        }
    }
}

GraphicObject::~GraphicObject()
{
    delete grid_vertex_array;
    delete grid_index_array;
    delete arrow_vertex_array;
}