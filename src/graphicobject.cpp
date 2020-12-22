#include"graphicobject.h"

void GraphicObject::GetArrowVertexArray()
{
    // write arrow arrays
    unsigned int lx = source->x_size - 2;
    unsigned int ly = source->y_size - 2;
    unsigned int lz = source->z_size - 2;

    float dx = 2/((float)lx + 2);
    float dy = 2/((float)ly + 2);
    float dz = 2/((float)lz + 2);

    float norm_coeff = (float)source->u->norm_coeff;

    for (unsigned int k = 0; k < lz; ++k)
    {
        for (unsigned int j = 0; j < ly; ++j)
        {
            for (unsigned int i = 0; i < lx; ++i)
            {
                unsigned int iterator = 14*(((ly*k) + j)*lx + i);
                Vector3<double>* flow = (*source->u)(i + 1, j + 1, k + 1);
                float magnitude = (float)flow->Magnitude()/norm_coeff;
                float scale_param = 1.5f;

                //arrow tails
                arrow_vertex_array[iterator] = ((float)i + 1.5f)*dx - 1.0f;
                arrow_vertex_array[iterator + 1] = ((float)j + 1.5f)*dy - 1.0f;
                arrow_vertex_array[iterator + 2] = ((float)k + 1.5f)*dz + 1.0f;

                //arrow heads
                arrow_vertex_array[iterator + 7] = arrow_vertex_array[iterator] 
                + scale_param*dx*(float)(*flow)(0)/norm_coeff;

                arrow_vertex_array[iterator + 8] = arrow_vertex_array[iterator + 1] 
                + scale_param*dy*(float)(*flow)(1)/norm_coeff;

                arrow_vertex_array[iterator + 9] = arrow_vertex_array[iterator + 2] 
                + scale_param*dz*(float)(*flow)(2)/norm_coeff;

                // colours
                arrow_vertex_array[iterator + 3] = 0.5f*magnitude;
                arrow_vertex_array[iterator + 4] = 0.0f;
                arrow_vertex_array[iterator + 5] = 1.0f - magnitude;
                arrow_vertex_array[iterator + 6] = magnitude;

                arrow_vertex_array[iterator + 10] = 0.5f + 0.5f*magnitude;
                arrow_vertex_array[iterator + 11] = 0.0f;
                arrow_vertex_array[iterator + 12] = 0.5f - 0.5f*magnitude;
                arrow_vertex_array[iterator + 13] = magnitude;
            }
        }
    }
}

void GraphicObject::OrientGrid(Quaternion<float> orient)
{
   // change basis vectors from screen space to world space
   // float z_displacement = 2.0f*((float)source->z_size - 2.0f)/((float)source->z_size + 2.0f);
   // orient grid
    float z_displacement = 2.0f;
    Quaternion<float> rotated(0.0f, 0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < n_grid_vertices; ++i)
    {
        rotated = (orient*Quaternion<float>(0.0,
                                     grid_vertex_array[7*i],
                                     grid_vertex_array[7*i + 1],
                                     grid_vertex_array[7*i + 2] - z_displacement))
                                    *orient.Conj();

        grid_vertex_array[7*i] = rotated.i_vector(0);
        grid_vertex_array[7*i + 1] = rotated.i_vector(1);
        grid_vertex_array[7*i + 2] = rotated.i_vector(2) + z_displacement;
    }
}
    void GraphicObject::OrientArrows()
    {
        float z_displacement = 2.0f;
        Quaternion<float> rotated(0.0f, 0.0f, 0.0f, 0.0f);

        for (unsigned int i = 0; i < n_arrow_vertices; ++i)
        {
            rotated = (orientation*Quaternion<float>(0.0,
                                        arrow_vertex_array[14*i],
                                        arrow_vertex_array[14*i + 1],
                                        arrow_vertex_array[14*i + 2] - z_displacement))
                                        *orientation.Conj();

            arrow_vertex_array[14*i] = rotated.i_vector(0);
            arrow_vertex_array[14*i + 1] = rotated.i_vector(1);
            arrow_vertex_array[14*i + 2] = rotated.i_vector(2) + z_displacement;

            rotated = (orientation*Quaternion<float>(0.0,
                                        arrow_vertex_array[14*i + 7],
                                        arrow_vertex_array[14*i + 8],
                                        arrow_vertex_array[14*i + 9] - z_displacement))
                                        *orientation.Conj();

            arrow_vertex_array[14*i + 7] = rotated.i_vector(0);
            arrow_vertex_array[14*i + 8] = rotated.i_vector(1);
            arrow_vertex_array[14*i + 9] = rotated.i_vector(2) + z_displacement;
        }
    }


GraphicObject::GraphicObject(Fluid& F)
:
source(&F),
orientation(1/sqrt(2), -1/sqrt(2), 0.0f, 0.0f)
{
    // assign vertices ignoring boundary layer
    unsigned int lx = source->x_size - 2;
    unsigned int ly = source->y_size - 2;
    unsigned int lz = source->z_size - 2;

    float dx = 2/((float)lx + 2);
    float dy = 2/((float)ly + 2);
    float dz = 2/((float)lz + 2);

    // write grid arrays
    n_grid_vertices = 2*(lx*ly + lx*lz + ly*lz + 1);
    n_grid_indices = 2*(lx*ly + lx*lz + ly*lz + 2*lx + 2*ly + 2*lz + 3);
    n_arrow_vertices = lx*ly*lz;

    grid_vertex_array = new float[7*n_grid_vertices];
    grid_index_array = new unsigned int[n_grid_indices];
    arrow_vertex_array = new float[14*n_arrow_vertices];

    unsigned int vertex_iterator = 0;
    unsigned int index_iterator = 0;

    for (unsigned int k = 0; k < lz + 1; ++k)
    {
        for (unsigned int j = 0; j < ly + 1; ++j)
        {
            for (unsigned int i = 0; i < lx + 1; ++i)
            {
                if (i == 0 || j == 0 || i == lx || j == ly)
                {
                    grid_vertex_array[7*vertex_iterator] = ((float)i + 1.0f)*dx - 1.0f;
                    grid_vertex_array[7*vertex_iterator + 1] = ((float)j + 1.0f)*dy - 1.0f;
                    grid_vertex_array[7*vertex_iterator + 2] = ((float)k + 1.0f)*dz + 1.0f;
                    grid_vertex_array[7*vertex_iterator + 3] = 0.2f;
                    grid_vertex_array[7*vertex_iterator + 4] = 0.2f;
                    grid_vertex_array[7*vertex_iterator + 5] = 0.25f;
                    grid_vertex_array[7*vertex_iterator + 6] = 0.25f;

                    if (i == 0)
                    {
                        grid_index_array[index_iterator] = vertex_iterator;
                        ++index_iterator;

                        if (j == 0 || j == ly)
                        {
                            grid_index_array[index_iterator] = vertex_iterator + lx;
                        }

                        else
                        {
                            grid_index_array[index_iterator] = vertex_iterator + 1;
                        }

                        ++index_iterator;
                    }

                    if (j == 0)
                    {
                        grid_index_array[index_iterator] = vertex_iterator;
                        ++index_iterator;
                        
                        if (i == 0 || i == lx)
                        {
                            grid_index_array[index_iterator] = vertex_iterator + lx + 2*ly - 1;
                        }

                        else 
                        {
                            grid_index_array[index_iterator] = vertex_iterator + lx + 2*ly - 1;
                        }

                        ++index_iterator;
                    }

                    if (k == 0)
                    {
                        grid_index_array[index_iterator] = vertex_iterator;
                        ++index_iterator;
                        grid_index_array[index_iterator] = vertex_iterator + 2*(lx + ly)*lz;
                        ++index_iterator;
                    }

                    ++vertex_iterator;
                }
            }
        }
    }

    for (unsigned int j = 1; j < ly; ++j)
    {
        for (unsigned int i = 1; i < lx; ++i)
        {
            grid_vertex_array[7*vertex_iterator] = ((float)i + 1.0f)*dx - 1.0f;
            grid_vertex_array[7*vertex_iterator + 1] = ((float)j + 1.0f)*dy - 1.0f;
            grid_vertex_array[7*vertex_iterator + 2] = (float)dz + 1.0f;
            grid_vertex_array[7*vertex_iterator + 3] = 0.2f;
            grid_vertex_array[7*vertex_iterator + 4] = 0.2f;
            grid_vertex_array[7*vertex_iterator + 5] = 0.25f;
            grid_vertex_array[7*vertex_iterator + 6] = 0.25f;

            grid_index_array[index_iterator] = vertex_iterator;
            ++index_iterator;
            ++vertex_iterator;
            
            grid_vertex_array[7*vertex_iterator] = ((float)i + 1.0f)*dx - 1.0f;
            grid_vertex_array[7*vertex_iterator + 1] = ((float)j + 1.0f)*dy - 1.0f;
            grid_vertex_array[7*vertex_iterator + 2] = (lz + 1.0f)*dz + 1.0f;
            grid_vertex_array[7*vertex_iterator + 3] = 0.4f;
            grid_vertex_array[7*vertex_iterator + 4] = 0.2f;
            grid_vertex_array[7*vertex_iterator + 5] = 0.25f;
            grid_vertex_array[7*vertex_iterator + 6] = 0.25f;

            grid_index_array[index_iterator] = vertex_iterator;
            ++index_iterator;
            ++vertex_iterator;
        }
    }

    GetArrowVertexArray();
}

GraphicObject::~GraphicObject()
{
    delete grid_vertex_array;
    delete grid_index_array;
    delete arrow_vertex_array;
}