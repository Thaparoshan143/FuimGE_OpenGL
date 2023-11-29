#pragma once

#include"./Types.h"

namespace Abstract
{
	// Buffer storage format - position..color..uv
	// 3 bit mask arrangment to find appropriate ..
	typedef enum BufferFormat
	{
		PP = 2,
		PPP = 3,
		PPPP = 4,

		PP_RGB = 26,
		PPP_RGB = 27,
		PPPP_RGB = 28,

		PP_RGBA = 34,
		PPP_RGBA = 35,
		PPPP_RGBA = 36,

		PP_RGB_UV = 154,
		PPP_RGB_UV = 155,
		PPPP_RGB_UV = 156,

	}BufferFormat;

	#define BUFFER_MASKPPP 0x00000007
	#define BUFFER_MASKRGB 0x00000038
	#define BUFFER_MASKTEX 0x000001C0

	template<typename T>
    class VertexBufferObject
    {
        public:
        VertexBufferObject(uint size = 2) {	m_data.reserve(size);	}

        virtual void LoadBuffer(uint loadMode) = 0;
        virtual void Append(T data) {  m_data.push_back(data);	};
        virtual void Append(std::vector<T> data) {  for(T _d:data) {	m_data.push_back(_d);	} };
		virtual uint Bind() = 0;
		virtual void Unbind() = 0;

        std::vector<T> m_data;
		uint m_id;
    };

    class VertexArrayObject
    {
        public:
        VertexArrayObject(BufferFormat bf = PP) {	m_format = bf;	}
        
		virtual uint Bind() = 0;
		virtual void Unbind() = 0;
        void UpdateFormat(BufferFormat bf) {    m_format = bf;  }

		BufferFormat m_format;
		uint m_id;

		uint getMask(uint maskFormat) { return m_format & maskFormat;   }
    };
}