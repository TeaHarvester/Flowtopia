#include"graphicobject.h"

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