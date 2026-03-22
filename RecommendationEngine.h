#pragma once
#include <vector>
#include <memory>
#include "Product.h"
#include "User.h"
#include "Rating.h"

class RecommendationEngine {
public:
    RecommendationEngine(std::vector<std::shared_ptr<Product>>& products,
        std::vector<std::shared_ptr<Rating>>& ratings);

    // Контентная фильтрация (по тегам)
    std::vector<std::shared_ptr<Product>> getContentBasedRecommendations(std::shared_ptr<User> user, int count);

    // Коллаборативная фильтрация (по похожим пользователям)
    std::vector<std::shared_ptr<Product>> getCollaborativeRecommendations(std::shared_ptr<User> user, int count);

    // Гибридный метод
    std::vector<std::shared_ptr<Product>> getHybridRecommendations(std::shared_ptr<User> user, int count);

private:
    std::vector<std::shared_ptr<Product>>& products_;
    std::vector<std::shared_ptr<Rating>>& ratings_;

    double calculateUserSimilarity(std::shared_ptr<User> u1, std::shared_ptr<User> u2);
    std::vector<std::shared_ptr<Product>> filterPurchased(std::shared_ptr<User> user,
        const std::vector<std::shared_ptr<Product>>& candidates);
};