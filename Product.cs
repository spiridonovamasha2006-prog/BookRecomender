using System;
using System.Collections.Generic;
using System.Linq;
using BookRecommender.Interfaces;

namespace BookRecommender.Models
{
    /// <summary>
    /// Базовый класс товара
    /// </summary>
    public abstract class Product : IPurchasable, IRatable, ISearchable, ICloneable
    {
        private static int _nextId = 1;

        public int Id { get; protected set; }
        public string Name { get; protected set; }
        public string Description { get; protected set; }
        public double Price { get; protected set; }
        public string Category { get; protected set; }

        protected readonly List<string> _tags = new();
        public IReadOnlyList<string> Tags => _tags.AsReadOnly();

        protected readonly List<int> _ratings = new();

        public Product()
        {
            Id = _nextId++;
        }

        public Product(string name, string description, double price, string category)
        {
            if (string.IsNullOrWhiteSpace(name))
                throw new ArgumentException("Name cannot be empty", nameof(name));
            if (price < 0)
                throw new ArgumentOutOfRangeException(nameof(price), "Price cannot be negative");
            if (string.IsNullOrWhiteSpace(category))
                throw new ArgumentException("Category cannot be empty", nameof(category));

            Id = _nextId++;
            Name = name;
            Description = description;
            Price = price;
            Category = category;
        }

        public virtual double GetPrice() => Price;

        public virtual string ToDisplayString() =>
            $"«{Name}» ({Category}) — {Price:F2}₽";

        public virtual double ApplyDiscount(double discountPercent)
        {
            if (discountPercent < 0 || discountPercent > 100)
                throw new ArgumentOutOfRangeException(nameof(discountPercent), "Discount must be 0-100");
            return Price * (1.0 - discountPercent / 100.0);
        }

        public void AddTag(string tag)
        {
            if (string.IsNullOrWhiteSpace(tag)) return;
            if (!_tags.Contains(tag))
                _tags.Add(tag);
        }

        public bool ContainsTag(string tag) => _tags.Contains(tag);

        public virtual bool Matches(string query)
        {
            if (string.IsNullOrWhiteSpace(query)) return false;
            var q = query.ToLowerInvariant();
            return Name.ToLowerInvariant().Contains(q) ||
                   Description.ToLowerInvariant().Contains(q) ||
                   _tags.Any(t => t.ToLowerInvariant().Contains(q));
        }

        public virtual double GetAverageRating()
        {
            return _ratings.Count == 0 ? 0.0 : _ratings.Average();
        }

        public virtual void AddRating(int score)
        {
            if (score < 1 || score > 5)
                throw new ArgumentOutOfRangeException(nameof(score), "Rating must be 1-5");
            _ratings.Add(score);
        }

        public virtual object Clone() => MemberwiseClone();

        public virtual Product DeepClone()
        {
            var clone = (Product)MemberwiseClone();
            clone._tags = new List<string>(_tags);
            clone._ratings = new List<int>(_ratings);
            return clone;
        }

        public static void ResetIdCounter(int start = 1) => _nextId = start;

        public override string ToString() => $"[Product {Id}] {Name} — {Price:F2}₽";
    }
}