#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Подключаем заголовки для работы с директориями
#ifdef _WIN32
#include <direct.h>
#include <sys/stat.h>
#ifndef S_IRUSR
#define S_IRUSR _S_IREAD
#endif
#ifndef S_IWUSR
#define S_IWUSR _S_IWRITE
#endif
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#endif

namespace Utils {
    std::string simpleHash(const std::string& input) {
        unsigned long hash = 5381;
        for (char c : input) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
        }
        return std::to_string(hash);
    }

    bool ensureDataDir() {
#ifdef _WIN32
        // Для Windows используем _mkdir
        int result = _mkdir("data");
        return result == 0 || errno == EEXIST;
#else
        // Для Linux/macOS используем mkdir
        int result = mkdir("data", 0755);
        return result == 0 || errno == EEXIST;
#endif
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
}