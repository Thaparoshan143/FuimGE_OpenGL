#ifdef TESTMODE
#pragma once

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

float* get_quad_bufferind_uv(fVec2 pos, fVec2 dim, Color3 col, uint uvCount = 2)
{
    float *temp = new float[QUAD_IND_VERTEX_COUNT + uvCount*4];
    float hhei = dim.y/2.0;
    float hwid = dim.x/2.0;
    int i = 0;

    *(temp+(6 + uvCount)*i) =  pos.x-hwid;
    *(temp+(6 + uvCount)*i+1) =  pos.y-hhei;
    *(temp+(6 + uvCount)*i+6) =  0;
    *(temp+(6 + uvCount)*i+7) =  0;

    *(temp+(6 + uvCount)*(++i)) = pos.x-hwid;
    *(temp+(6 + uvCount)*i+1) = pos.y+hhei;
    *(temp+(6 + uvCount)*i+6) =  0;
    *(temp+(6 + uvCount)*i+7) =  1;

    *(temp+(6 + uvCount)*(++i)) = pos.x+hwid;
    *(temp+(6 + uvCount)*i+1) = pos.y-hhei;
    *(temp+(6 + uvCount)*i+6) =  1;
    *(temp+(6 + uvCount)*i+7) =  0;

    *(temp+(6 + uvCount)*(++i)) =  pos.x+hwid;
    *(temp+(6 + uvCount)*i+1) =  pos.y+hhei;
    *(temp+(6 + uvCount)*i+6) =  1;
    *(temp+(6 + uvCount)*i+7) =  1;

    for(i=0;i<QUAD_IND_VERTEX_COUNT/PER_VERTEX_MOD;++i)
    {
        *(temp+(6 + uvCount)*i+3) = col.r;
        *(temp+(6 + uvCount)*i+4) = col.g;
        *(temp+(6 + uvCount)*i+5) = col.b;
    }

    return temp;
}

void print_float(float *data, uint count, char seperator = '\n')
{
    for(uint i=0;i<count;++i)
    {
        std::cout << *(data+i) << " " << seperator;
    }
}

void seperator(const char *s, uint count)
{
    for(uint i=0;i<count;++i)
    {
        std::cout << s;
    }
    std::cout << std::endl;
}

float* get_grid_pointxy_ind(fVec2 start, fVec2 end, fVec2 gridCount)
{
    int pointCount = ((gridCount.x + 2 + gridCount.y + 2)*2)*3;
    // std::cout << "Point Count : " << pointCount << std::endl;
    float *temp = new float[pointCount];
    fVec2 offset = fVec2((end.x - start.x) / (gridCount.x + 1), (end.y - start.y) / (gridCount.y + 1));
    // std::cout << "Offset : " << offset.x << ", " << offset.y << std::endl;

    // populating x coordinates
    for(int i = 0; i<gridCount.x+2;++i)
    {
        *(temp++) = start.x + i * offset.x;
        *(temp++);
        *(temp++) = start.y;
        *(temp++) = start.x + i * offset.x;
        *(temp++);
        *(temp++) = end.y;
        // std::cout << *(temp-2) << "\t, " << *(temp-1) << std::endl;
    }
    // seperator("-", 100);

    // populating y coordinates
    for(int i = 0; i<gridCount.y+2;++i)
    {
        *(temp++) = start.x;
        *(temp++);
        *(temp++) = start.y + i * offset.y;
        *(temp++) = end.x;
        *(temp++);
        *(temp++) = start.y + i * offset.y;
        // std::cout << *(temp-2) << "\t, " << *(temp-1) << std::endl;
    }
    // seperator("-", 100);
    
    // std::cout << " => first : " << *(temp-pointCount) << std::endl;
    // seperator(" * ",30);
    return (temp-pointCount);
}

uint get_grid_xy_size(fVec2 gridCount)
{
    return ((gridCount.x + 2 + gridCount.y + 2)*2)*3;
}

void print_point_2d(float *data, uint pointCount)
{
    for(int i=0;i<pointCount/2;i++)
    {
        std::cout <<  "X : " << *(data++) << "\t, Y : " << *(data++) << std::endl;
    }
}

#endif