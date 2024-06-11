#pragma once

#include"./Types.h"

#define DEF_SHA_PATH "../res/Shaders/"
#define DEF_VERT_FILE "vert.shader"
#define DEF_FRAG_FILE "frag.shader"

namespace Interface
{
    class IShader
    {
        public:
        IShader(String dir = DEF_SHA_PATH, String vertFile = DEF_VERT_FILE, String fragFile = DEF_FRAG_FILE) : m_programHandle(0), m_vertexHandle(0), m_fragmentHandle(0)    {   m_dir = dir;    m_vertFile = vertFile;  m_fragFile = fragFile;    }

        virtual uint CreateProgram() = 0;
        virtual bool CompileProgram() = 0;
        // virtual bool Recompile() = 0;
        virtual void DeleteProgram() = 0;
        virtual void UseProgram() = 0;
        // void UpdateDirFile(String dir, String vertFile, String fragFile) {   m_dir = dir;    m_vertFile = vertFile;  m_fragFile = fragFile;  Recompile();    }
        inline uint GetProgramHandle()  {   return m_programHandle;    }
        inline uint GetVertexHandle()  {   return m_vertexHandle;    }
        inline uint GetFragmentHandle()  {   return m_fragmentHandle;    }

        protected:
        String m_dir, m_vertFile, m_fragFile;
        uint m_programHandle, m_vertexHandle, m_fragmentHandle;
    };
}