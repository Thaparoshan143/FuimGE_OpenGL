#ifndef VERTEXGENERATOR_CPP
#define VERTEXGENERATOR_CPP

#include<structure.cpp>
#include<iomanip> 

#define TRIANGLE_VERTEX_COUNT 18
#define QUAD_VERTEX_COUNT 36 // using two triangle method
#define QUAD_IND_VERTEX_COUNT 24 // using Index method
#define PER_VERTEX_MOD 6
#define QUAD_INDEX_COUNT 6

namespace Util
{
    float* get_triangle_buffer(fVec3 pos, fVec3 dim, Color4 col)
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

    uint32_t* get_quad_index()
    {
        uint32_t *temp = new uint32_t[QUAD_INDEX_COUNT];

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

    float* get_quad_bufferind_uv(fVec2 pos, fVec2 dim, Color3 col, uint32_t uvCount = 2)
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

    void print_float_xyz(float *data, uint32_t count, char seperator = '\n')
    {
        for(uint32_t i=0;i<count;i+=3)
        {
            std::cout << std::setprecision(5) << *(data+i) << ",\t" << *(data+i+1) << ",\t" << *(data+i+2) << seperator;
        }
    }

    void print_float_xy(float *data, uint32_t count, char seperator = '\n')
    {
        for(uint32_t i=0;i<count;i+=2)
        {
            std::cout << std::setprecision(5) << *(data+i) << ",\t" << *(data+i+1) << seperator;
        }
    }
}
#endif