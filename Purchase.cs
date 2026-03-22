using System;
using System.Collections.Generic;
using System.Linq;

namespace BookRecommender.Models
{
    /// <summary>
    /// Класс завершенной покупки
    /// </summary>
    public class Purchase
    {
        private static int _nextId = 1;

        public int Id { get; private set; }
        public User User { get; private set; }
        public DateTime PurchaseDate { get; private set; }

        private readonly List<(Product Product, int Quantity)> _products = new();
        public IReadOnlyList<(Product Product, int Quantity)> Products => _products.AsReadOnly();

        public double TotalAmount =>
            _products.Sum(p => p.Product.GetPrice() * p.Quantity);

        public Purchase(User user, DateTime? purchaseDate = null)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));

            Id = _nextId++;
            User = user;
            PurchaseDate = purchaseDate ?? DateTime.Now;
        }

        public void AddProduct(Product product, int quantity)
        {
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            if (quantity <= 0)
                throw new ArgumentOutOfRangeException(nameof(quantity), "Quantity must be > 0");

            _products.Add((product, quantity));
        }

        public override string ToString() =>
            $"Purchase #{Id} by {User.Name} — {TotalAmount:F2}₽ ({Products.Count} items)";
    }
}