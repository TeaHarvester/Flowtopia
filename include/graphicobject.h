#ifndef GRAPHICOBJECT
#define GRAPHICOBJECT

#include<iostream>
#include"helpers/quaternion.h"
#include"fluid.h"

struct GraphicObject
{
    unsigned int n_grid_vertices;
    unsigned int n_grid_indices; 
    unsigned int n_arrow_vertices;
    float* grid_vertex_array;
    unsigned int* grid_index_array;
    float* arrow_vertex_array;
    Fluid* source;
    Quaternion<float> orientation;
    void GetArrowVertexArray();
    void Orient();
    GraphicObject(Fluid& f);
    ~GraphicObject();
};

#endif