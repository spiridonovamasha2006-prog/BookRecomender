#include "RecommendationEngine.h"
#include <algorithm>
#include <set>

RecommendationEngine::RecommendationEngine(std::vector<std::shared_ptr<Product>>& products,
    std::vector<std::shared_ptr<Rating>>& ratings)
    : products_(products), ratings_(ratings) {
}

std::vector<std::shared_ptr<Product>> RecommendationEngine::getContentBasedRecommendations(std::shared_ptr<User> user, int count) {
    // Упрощенная логика: ищем товары с тегами, которые встречаются в купленных товарах
    std::vector<std::string> preferredTags;
    // В реальной системе здесь анализ истории покупок user->getPurchaseHistory()
    // Для демо возьмем все товары и отфильтруем по популярным тегам
    std::vector<std::shared_ptr<Product>> result;
    for (const auto& p : products_) {
        if (p->containsTag("book") || p->containsTag("ebook")) {
            result.push_back(p);
        }
    }
    return filterPurchased(user, result);
}

std::vector<std::shared_ptr<Product>> RecommendationEngine::getCollaborativeRecommendations(std::shared_ptr<User> user, int count) {
    // Упрощенная логика: возвращаем товары с высоким средним рейтингом
    std::vector<std::shared_ptr<Product>> result = products_;
    // Сортировка по рейтингу (заглушка, так как рейтинг хранится отдельно)
    return filterPurchased(user, result);
}

std::vector<std::shared_ptr<Product>> RecommendationEngine::getHybridRecommendations(std::shared_ptr<User> user, int count) {
    auto content = getContentBasedRecommendations(user, count);
    auto collab = getCollaborativeRecommendations(user, count);

    // Объединение
    std::set<int> ids;
    std::vector<std::shared_ptr<Product>> result;
    for (const auto& p : content) {
        if (ids.find(p->getId()) == ids.end()) {
            result.push_back(p);
            ids.insert(p->getId());
        }
    }
    for (const auto& p : collab) {
        if (ids.find(p->getId()) == ids.end()) {
            result.push_back(p);
            ids.insert(p->getId());
        }
    }
    return result;
}

double RecommendationEngine::calculateUserSimilarity(std::shared_ptr<User> u1, std::shared_ptr<User> u2) {
    // Заглушка для косинусной меры
    return 0.5;
}

std::vector<std::shared_ptr<Product>> RecommendationEngine::filterPurchased(std::shared_ptr<User> user,
    const std::vector<std::shared_ptr<Product>>& candidates) {
    // Исключаем уже купленные (упрощенно)
    return candidates;
}