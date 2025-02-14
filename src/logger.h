#pragma once

#include <iostream>

namespace logger {
    struct Channel {
        std::string name;
        Channel(const std::string& name) : name(name) {}

        void trace(const std::string& content) {
            #ifndef NDEBUG
                std::cout << "DEBUG  " << channel_prefix(name) << content << std::endl;
            #endif
        }
    
        void info(const std::string& content) {
            std::cout << "INFO   " << channel_prefix(name) << content << std::endl;
        }
    
        void warn(const std::string& content) {
            std::cout << "WARN   " << channel_prefix(name) << content << std::endl;
        }
    
        void error(const std::string& content) {
            std::cout << "ERROR  " << channel_prefix(name) << content << std::endl;
        }
    
        std::string prompt(const std::string& str) {
            std::string result;
            std::cout << "PROMPT " << channel_prefix(name) << " >> " << str << ": ";
            std::getline(std::cin, result);
            return result;
        }
    private:
        static std::string channel_prefix(const std::string& name) {
            return "[" + name + "] ";
        }
    };

    Channel log(const std::string& chan);
};

