#pragma once

#include"./Types.h"

namespace Log
{
    typedef enum LogType
    {
        TERMINAL = 0, 
        FILE_OVERWRITE = 1,
        FILE_APPEND = 2,
        TERMINAL_AND_FILE_OVERWRITE = 3,
        TERMINAL_AND_FILE_APPEND = 4,
    }LogType;

    typedef enum LogDetail
    {
        // Just the required text
        BASIC = 0,
        TIME_STAMP = 1,
    }LogDetail;

    class Debug
    {
        public:
        Debug();

        void SetLogFile(String logFileName)
        {
            m_logFileName = logFileName;
        }

        void LogOut()
        {
            switch(m_logType)
            {
                case TERMINAL:
                terminalLog();
                break;
                case FILE_APPEND:
                fileLog(std::ios_base::app);
                break;
                case FILE_OVERWRITE:
                fileLog(std::ios_base::out);
                break;
                case TERMINAL_AND_FILE_APPEND:
                terminalLog();
                fileLog(std::ios_base::app);
                case TERMINAL_AND_FILE_OVERWRITE:
                terminalLog();
                fileLog(std::ios_base::out);
                break;
                default:
                std::cout << "Cannot find the required type" << std::endl;
                break;
            }
        }

        protected:
        String m_logFileName;
        LogType m_logType;
        LogDetail m_logDetail;
        String m_logBuffer;

        void terminalLog()
        {
            
        }

        void fileLog(uint mode)
        {
            
        }
    };

    static inline void TLogStr(String title, char endOfLine = '\n')
    {
        std::cout << title << endOfLine;
    }

    template<typename T>
    static void TLogIsPointerNull(T target)
    {
        target == nullptr ? TLogStr("=> Yes") : TLogStr("=> No");
    }
}