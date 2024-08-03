#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <streambuf>

class Logger {
public:
    enum class LogLevel {
        INFO,
        WARNING,
        ERROR,
        NEWLOG
    };

    

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance(const std::string& filename = "application.log") {
        static Logger instance(filename, 1048576, 5); // Guaranteed to be destroyed and instantiated on first use
        return instance;
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);

        if (currentFileSize >= maxFileSize) {
            rotateLogFile();
        }

        std::string levelStr;
        switch (level) {
            case LogLevel::INFO:    levelStr = "INFO";    break;
            case LogLevel::WARNING: levelStr = "WARNING"; break;
            case LogLevel::ERROR:   levelStr = "ERROR";   break;
            case LogLevel::NEWLOG:  levelStr = "NEWLOG";  break;
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::string timeStr = std::ctime(&time);
        timeStr.pop_back(); // Remove trailing newline

        if (logFile.is_open()) {
            logFile << "[" << timeStr << "] [" << levelStr << "] " << message << std::endl;
            logFile.flush(); // Ensure the log is written to the file immediately
            currentFileSize += (message.length() + levelStr.length() + 100); // Rough estimate
        } else {
            std::cerr << "Log file is not open!" << std::endl;
        }
    }

    void setStreamOutput(std::ostream& os, LogLevel level) {
        os.rdbuf(new StreamLogger(this, level));
    }

private:
    std::string baseFilename;
    std::ofstream logFile;
    size_t maxFileSize;
    size_t maxFiles;
    size_t currentFileSize;
    std::mutex mtx;

    Logger(const std::string& filename, size_t maxFileSize, size_t maxFiles)
        : baseFilename(filename), maxFileSize(maxFileSize), maxFiles(maxFiles), currentFileSize(0) {
        rotateLogFile();
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    class StreamLogger : public std::streambuf {
    public:
        StreamLogger(Logger* logger, LogLevel level)
            : logger(logger), level(level) {}

    protected:
        int_type overflow(int_type ch) override {
            if (ch != traits_type::eof()) {
                buffer += static_cast<char>(ch);
                if (ch == '\n') {
                    logger->log(level, buffer);
                    buffer.clear();
                }
            }
            return ch;
        }

        int sync() override {
            if (!buffer.empty()) {
                logger->log(level, buffer);
                buffer.clear();
            }
            return 0;
        }

    private:
        Logger* logger;
        LogLevel level;
        std::string buffer;
    };

    void rotateLogFile() {
        if (logFile.is_open()) {
            logFile.close();
        }

        for (size_t i = maxFiles; i > 1; --i) {
            std::string oldFilename = baseFilename + "." + std::to_string(i - 1);
            std::string newFilename = baseFilename + "." + std::to_string(i);
            if (std::remove(newFilename.c_str()) == 0) {
                std::rename(oldFilename.c_str(), newFilename.c_str());
            }
        }

        std::string currentFilename = baseFilename + ".0";
        if (std::remove(currentFilename.c_str()) == 0) {
            std::rename(baseFilename.c_str(), currentFilename.c_str());
        }

        logFile.open(baseFilename, std::ios::app);
        currentFileSize = 0;
    }
};

#endif