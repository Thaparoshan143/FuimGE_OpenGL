#pragma once

#include"./Types.h"
#include"../../includes/stb/stb_image.h"

namespace Image
{
    class ImageLoader
    {
        public:
        ImageLoader(String path) : m_path(path) {   }

        void SetImageSource(String path)    {   m_path = path;  }
        iVec2 GetImageDim() {   return m_imgDim;    }

        uchar* GetImageData()
        {
            try
            {
                return stbi_load(m_path.c_str(), &m_imgDim.x, &m_imgDim.y, &m_channelCount, 0);
            }
            catch(...)
            {
                std::cout << "Unable to resolve either image or directory";
                return nullptr;
            }
        }

        protected:
        String m_path;
        iVec2 m_imgDim;
        int m_channelCount;
    };
}