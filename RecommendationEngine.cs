using System;
using System.Collections.Generic;
using System.Linq;
using BookRecommender.Models;

namespace BookRecommender.Services
{
    /// <summary>
    /// Движок рекомендательной системы
    /// </summary>
    public class RecommendationEngine
    {
        private readonly List<Product> _products;
        private readonly List<Rating> _ratings;
        private readonly List<User> _users;

        public RecommendationEngine(List<Product> products, List<Rating> ratings, List<User> users)
        {
            _products = products ?? throw new ArgumentNullException(nameof(products));
            _ratings = ratings ?? throw new ArgumentNullException(nameof(ratings));
            _users = users ?? throw new ArgumentNullException(nameof(users));
        }

        /// <summary>
        /// Контентная фильтрация (по тегам и категориям)
        /// </summary>
        public List<Product> GetContentBasedRecommendations(User user, int count = 5)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));

            // Собираем теги из купленных книг
            var preferredTags = new HashSet<string>();
            foreach (var purchase in user.PurchaseHistory)
            {
                foreach (var item in purchase.Products)
                {
                    foreach (var tag in item.Product.Tags)
                    {
                        preferredTags.Add(tag);
                    }
                }
            }

            // Находим книги с похожими тегами
            var scored = new Dictionary<Product, int>();
            foreach (var product in _products)
            {
                int score = 0;
                foreach (var tag in product.Tags)
                {
                    if (preferredTags.Contains(tag))
                        score++;
                }
                if (score > 0)
                    scored[product] = score;
            }

            return scored.OrderByDescending(x => x.Value)
                        .Select(x => x.Key)
                        .Take(count)
                        .ToList();
        }

        /// <summary>
        /// Коллаборативная фильтрация (по похожим пользователям)
        /// </summary>
        public List<Product> GetCollaborativeRecommendations(User user, int count = 5)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));

            // Находим похожих пользователей
            var similarUsers = new Dictionary<User, double>();
            foreach (var otherUser in _users)
            {
                if (otherUser.Id != user.Id)
                {
                    double similarity = CalculateUserSimilarity(user, otherUser);
                    if (similarity > 0)
                        similarUsers[otherUser] = similarity;
                }
            }

            // Собираем книги, которые понравились похожим пользователям
            var productScores = new Dictionary<Product, double>();
            foreach (var similarUser in similarUsers)
            {
                foreach (var purchase in similarUser.Key.PurchaseHistory)
                {
                    foreach (var item in purchase.Products)
                    {
                        if (!productScores.ContainsKey(item.Product))
                            productScores[item.Product] = 0;
                        productScores[item.Product] += similarUser.Value * item.Product.GetAverageRating();
                    }
                }
            }

            return productScores.OrderByDescending(x => x.Value)
                               .Select(x => x.Key)
                               .Take(count)
                               .ToList();
        }

        /// <summary>
        /// Гибридные рекомендации (комбинация обоих методов)
        /// </summary>
        public List<Product> GetHybridRecommendations(User user, int count = 5)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));

            var contentRecs = GetContentBasedRecommendations(user, count * 2);
            var collabRecs = GetCollaborativeRecommendations(user, count * 2);

            // Объединяем и убираем дубликаты
            var allRecs = new Dictionary<Product, int>();
            foreach (var p in contentRecs)
            {
                if (!allRecs.ContainsKey(p))
                    allRecs[p] = 0;
                allRecs[p] += 2; // Контентная фильтрация имеет больший вес
            }
            foreach (var p in collabRecs)
            {
                if (!allRecs.ContainsKey(p))
                    allRecs[p] = 0;
                allRecs[p] += 1;
            }

            return allRecs.OrderByDescending(x => x.Value)
                         .Select(x => x.Key)
                         .Take(count)
                         .ToList();
        }

        /// <summary>
        /// Расчет схожести пользователей по оценкам
        /// </summary>
        private double CalculateUserSimilarity(User u1, User u2)
        {
            var u1Ratings = new Dictionary<int, int>();
            var u2Ratings = new Dictionary<int, int>();

            foreach (var rating in _ratings.Where(r => r.User.Id == u1.Id))
            {
                u1Ratings[rating.Product.Id] = rating.Score;
            }
            foreach (var rating in _ratings.Where(r => r.User.Id == u2.Id))
            {
                u2Ratings[rating.Product.Id] = rating.Score;
            }

            // Находим общие оцененные книги
            var commonProducts = u1Ratings.Keys.Intersect(u2Ratings.Keys).ToList();
            if (!commonProducts.Any())
                return 0;

            // Косинусная мера схожести
            double dotProduct = 0;
            double norm1 = 0;
            double norm2 = 0;

            foreach (var productId in commonProducts)
            {
                dotProduct += u1Ratings[productId] * u2Ratings[productId];
                norm1 += u1Ratings[productId] * u1Ratings[productId];
                norm2 += u2Ratings[productId] * u2Ratings[productId];
            }

            if (norm1 == 0 || norm2 == 0)
                return 0;

            return dotProduct / (Math.Sqrt(norm1) * Math.Sqrt(norm2));
        }

        public override string ToString() =>
            $"RecommendationEngine — {_products.Count} products, {_ratings.Count} ratings, {_users.Count} users";
    }
}