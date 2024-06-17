#ifdef TESTMODE

#include<iostream>

#include"../Core/Base/Types.h"

#define TRIANGLE_VERTEX_COUNT 18
#define QUAD_VERTEX_COUNT 36 // using two triangle method
#define QUAD_IND_VERTEX_COUNT 24 // using Index method
#define PER_VERTEX_MOD 6
#define QUAD_INDEX_COUNT 6

float* get_triangle_buffer(fVec2 pos, fVec2 dim, Color3 col)
{
    float *temp = new float[TRIANGLE_VERTEX_COUNT];
    float hhei = dim.y/2.0;
    float hwid = dim.x/2.0;
    int i = 0;

    *(temp+6*i) =  pos.x-hwid;
    *(temp+6*i+1) =  pos.y-hhei;

    *(temp+6*(++i)) = pos.x+hwid;
    *(temp+6*i+1) =  pos.y-hhei;

    *(temp+6*(++i)) = pos.x;
    *(temp+6*i+1) =  pos.y+hhei;

    for(i=0;i<TRIANGLE_VERTEX_COUNT/PER_VERTEX_MOD;++i)
    {
        *(temp+6*i+3) = col.r;
        *(temp+6*i+4) = col.g;
        *(temp+6*i+5) = col.b;
    }

    return temp;
}

float* get_quad_buffer(fVec2 pos, fVec2 dim, Color3 col)
{
    float *temp = new float[QUAD_VERTEX_COUNT];
    float hhei = dim.y/2.0;
    float hwid = dim.x/2.0;
    int i = 0;

    *(temp+6*i) =  pos.x-hwid;
    *(temp+6*i+1) =  pos.y-hhei;

    *(temp+6*(++i)) = *(temp+6*4) = pos.x-hwid;
    *(temp+6*i+1) =  *(temp+6*4+1) = pos.y+hhei;

    *(temp+6*(++i)) = *(temp+6*5)= pos.x+hwid;
    *(temp+6*i+1) =  *(temp+6*5+1) = pos.y-hhei;

    *(temp+6*(++i)) =  pos.x+hwid;
    *(temp+6*i+1) =  pos.y+hhei;

    for(i=0;i<QUAD_VERTEX_COUNT/PER_VERTEX_MOD;++i)
    {
        *(temp+6*i+3) = col.r;
        *(temp+6*i+4) = col.g;
        *(temp+6*i+5) = col.b;
    }

    return temp;
}

uint* get_quad_index()
{
    uint *temp = new uint[QUAD_INDEX_COUNT];

    *(temp+0) = 0;
    *(temp+1) = *(temp+3) = 1;
    *(temp+2) = *(temp+4) = 2;
    *(temp+5) = 3;

    return temp;
}

float* get_quad_bufferind(fVec2 pos, fVec2 dim, Color3 col)
{
    float *temp = new float[QUAD_IND_VERTEX_COUNT];
    float hhei = dim.y/2.0;
    float hwid = dim.x/2.0;
    int i = 0;

    *(temp+6*i) =  pos.x-hwid;
    *(temp+6*i+1) =  pos.y-hhei;

    *(temp+6*(++i)) = pos.x-hwid;
    *(temp+6*i+1) = pos.y+hhei;

    *(temp+6*(++i)) = pos.x+hwid;
    *(temp+6*i+1) = pos.y-hhei;

    *(temp+6*(++i)) =  pos.x+hwid;
    *(temp+6*i+1) =  pos.y+hhei;

    for(i=0;i<QUAD_IND_VERTEX_COUNT/PER_VERTEX_MOD;++i)
    {
        *(temp+6*i+3) = col.r;
        *(temp+6*i+4) = col.g;
        *(temp+6*i+5) = col.b;
    }

    return temp;
}

void print_float(float *data, uint count, char seperator='\n')
{
    for(uint i=0;i<count;++i)
    {
        std::cout << *(data+i) << " " << seperator;
    }
}
#endif