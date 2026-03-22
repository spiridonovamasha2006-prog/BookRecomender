using System;
using System.Collections.Generic;
using System.Linq;

namespace BookRecommender.Models
{
    /// <summary>
    /// Класс корзины покупок
    /// </summary>
    public class Cart : IDisposable
    {
        private static int _nextId = 1;

        public int Id { get; private set; }
        public User User { get; private set; }

        private readonly List<(Product Product, int Quantity)> _items = new();
        public IReadOnlyList<(Product Product, int Quantity)> Items => _items.AsReadOnly();

        private bool _disposed = false;

        public Cart(User user)
        {
            if (user == null)
                throw new ArgumentNullException(nameof(user));

            User = user;
            Id = _nextId++;
        }

        public void AddItem(Product product, int quantity = 1)
        {
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            if (quantity <= 0)
                throw new ArgumentOutOfRangeException(nameof(quantity), "Quantity must be > 0");

            var existing = _items.FirstOrDefault(i => i.Product.Id == product.Id);
            if (existing.Product != null)
            {
                _items.Remove(existing);
                _items.Add((existing.Product, existing.Quantity + quantity));
            }
            else
            {
                _items.Add((product, quantity));
            }
        }

        public void RemoveItem(Product product)
        {
            if (product == null) return;
            _items.RemoveAll(item => item.Product.Id == product.Id);
        }

        public double GetTotalPrice() =>
            _items.Sum(i => i.Product.GetPrice() * i.Quantity);

        public Purchase Checkout()
        {
            if (!_items.Any())
                throw new InvalidOperationException("Cannot checkout empty cart");

            var purchase = new Purchase(User);
            foreach (var item in _items)
            {
                purchase.AddProduct(item.Product, item.Quantity);
            }
            _items.Clear();
            return purchase;
        }

        public bool IsEmpty() => !_items.Any();

        public void Dispose()
        {
            if (!_disposed)
            {
                _items.Clear();
                _disposed = true;
            }
        }

        public override string ToString() =>
            $"Cart #{Id} for {User.Name} — {GetTotalPrice():F2}₽ ({_items.Count} items)";
    }
}