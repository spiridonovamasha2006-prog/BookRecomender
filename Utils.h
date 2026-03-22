#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>

// Forward declaration класса Product
class Product;

namespace Utils {
    // Простое хеширование паролей (для учебных целей)
    std::string simpleHash(const std::string& input);

    // Работа с файлами
    bool ensureDataDir();
    std::vector<std::string> split(const std::string& str, char delimiter);

    // Шаблонные функции для работы с файлами
    // Реализация должна быть в заголовочном файле!
    template<typename T>
    void saveToFile(const std::string& filename, const std::vector<std::shared_ptr<T>>& items) {
        std::ofstream file("data/" + filename);
        if (file.is_open()) {
            file << items.size() << std::endl;
            // Здесь должна быть логика сериализации конкретного типа T
            file.close();
        }
    }

    template<typename T>
    void loadFromFile(const std::string& filename, std::vector<std::shared_ptr<T>>& items) {
        std::ifstream file("data/" + filename);
        if (file.is_open()) {
            size_t count;
            file >> count;
            // Здесь должна быть логика десериализации
            file.close();
        }
    }
}