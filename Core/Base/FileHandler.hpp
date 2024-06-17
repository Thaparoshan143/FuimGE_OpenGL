#pragma once

#include"./Types.h"

#include <ios>
#include <fstream>
#include <sstream>

namespace File 
{
    // for now name is FileStream for read, write, append operation to any file using inbuild file handle with wrapper
    class InpFileStream
    {
        public:
        InpFileStream(String fileName, bool shouldIntactInBuffer = false)
        {
            m_readFileName = fileName;
            m_shouldIntactInBuffer = shouldIntactInBuffer;
            m_readBufferData.clear();
        }

        String ReadString()
        {
            if(m_shouldIntactInBuffer && m_readBufferData.size() != 0)
            {
                // std::cout << "Buffer Hit" << std::endl;
                return m_readBufferData;
            }
            else
            {
                std::stringstream fileStream = getInpFileStream(m_readFileName);
                return fileStream.str();
            }
        }

        const char* ReadRaw() 
        {   
            String tempStr = ReadString();
            char *raw = new char[tempStr.size()];
            memccpy(raw, tempStr.data(), sizeof(char), tempStr.size());
            return raw;
        }

        void SetInpFileName(String readFileName)   {   m_readFileName = readFileName;  m_readBufferData.clear();   }
        const String& GetInpFileName() const {   return m_readFileName;  }
        void SetInpBufferIntact(bool shouldIntactInBuffer) {   m_shouldIntactInBuffer = shouldIntactInBuffer;  }
        void ClearInpBuffer()  {   m_readBufferData.clear();   }
        const String& GetInpBufferDataHandle() const    {   return m_readBufferData;    }

        protected:
        String m_readFileName;
        String m_readBufferData;
        bool m_shouldIntactInBuffer;

        std::stringstream getInpFileStream(String &fileName)
        {
            // given openMode to be from ios_base::openmode
            std::ifstream tempStream(fileName, std::ios_base::in);
            std::stringstream tempBuffer;
            if(!tempStream.is_open())
            {
                std::cout << "Failed to load file for read !" << std::endl;
            }
            tempBuffer << tempStream.rdbuf();
            if(m_shouldIntactInBuffer)
            {
                m_readBufferData = tempBuffer.str();
            }
            tempStream.close();
            return tempBuffer;
        }
    };

    class OutFileStream
    {
        public:
        OutFileStream(String fileName)
        {
            m_writeFileName = fileName;
            m_writeBufferData.clear();
        }

        void WriteRaw(const char *data, uint size) 
        {
            std::ofstream tempStream(m_writeFileName, std::ios_base::out);
            if(!tempStream.is_open())
            {
                std::cout << "Unable to open File for write ! " << std::endl;
                return;
            }

            if(*data == '\0')
            {
                std::cout << "Warning : Written Data is empty " << std::endl;
            }

            tempStream.write(data, size);
            tempStream.close();
        }

        void WriteString(String &data)  {   WriteRaw(data.data(), data.size()); }
        void AppendOutBuffer(String data)  {   m_writeBufferData += data;  }
        void AppendOutBuffer(const char *data) {   m_writeBufferData.append(data); }
        void ClearOutBuffer()  {   m_writeBufferData.clear();  }
        void WriteFromOutBuffer()  {   WriteRaw(m_writeBufferData.data(), m_writeBufferData.size());   }
        void SetOutFileName(String writeFileName)  {   m_writeFileName = writeFileName;    m_writeBufferData.clear();  }
        const String& GetOutFileName() const {   return m_writeFileName; }
        const String& GetOutBufferDataHandle() const  {   return m_writeBufferData;   }

        protected:
        String m_writeFileName;
        String m_writeBufferData;
    };

    class MultiFileStream : public InpFileStream, public OutFileStream
    {
        public:
        MultiFileStream(String inFile, String outFile, bool intactInpBuffer = false) : InpFileStream(inFile, intactInpBuffer), OutFileStream(outFile)
        {

        }

        void CopyInputBufferToOutputBuffer()
        {
            if(!m_shouldIntactInBuffer)
            {
                std::cout << "input buffer is not intact" << std::endl;
                return;
            }
            else
            {
                m_writeBufferData = m_readBufferData;
            }
        }

        void SwapInpAndOutBuffer()
        {
            if(!m_shouldIntactInBuffer)
            {
                std::cout << "input buffer is not intact ! can't swap" << std::endl;
                return;
            }
            else
            {
                String temp = m_readBufferData;
                m_readBufferData = m_writeBufferData;
                m_writeBufferData = temp;
            }
        }

        void SwapInpAndOutFile()
        {
            String temp = m_readFileName;
            m_readFileName = m_writeFileName;
            m_writeFileName = temp;
        }
        
        void OffloadInpFileToOutput()
        {
            if(!m_shouldIntactInBuffer || m_readBufferData.size() == 0)
            {
                String temp = InpFileStream::ReadString();
                OutFileStream::WriteString(temp);
            }
            else
            {
                WriteString(m_readBufferData);
            }
        }

        protected:
        
    };
};