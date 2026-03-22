using System;

namespace BookRecommender.Models
{
    /// <summary>
    /// Класс оценки товара
    /// </summary>
    public class Rating
    {
        private static int _nextId = 1;

        public int Id { get; private set; }
        public User User { get; private set; }
        public Product Product { get; private set; }
        public int Score { get; private set; }
        public DateTime Timestamp { get; private set; }

        public Rating() { }

        public Rating(int id, User user, Product product, int score, DateTime? timestamp = null)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            if (score < 1 || score > 5)
                throw new ArgumentOutOfRangeException(nameof(score), "Score must be 1-5");

            Id = id;
            User = user;
            Product = product;
            Score = score;
            Timestamp = timestamp ?? DateTime.Now;
        }

        public Rating(User user, Product product, int score, DateTime? timestamp = null)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            if (score < 1 || score > 5)
                throw new ArgumentOutOfRangeException(nameof(score), "Score must be 1-5");

            Id = _nextId++;
            User = user;
            Product = product;
            Score = score;
            Timestamp = timestamp ?? DateTime.Now;
        }

        public static void ResetIdCounter(int start = 1) => _nextId = start;

        public override string ToString() =>
            $"Rating #{Id}: {User.Name} → {Product.Name} = {Score}";
    }
}