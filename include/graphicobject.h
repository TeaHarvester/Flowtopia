#ifndef GRAPHICOBJECT
#define GRAPHICOBJECT

#include<iostream>
#include"fluid.h"

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

#endif