using System;
using System.Collections.Generic;
using System.Linq;
using BookRecommender.Models;

namespace BookRecommender.Services
{
    /// <summary>
    /// Шаблонный класс каталога товаров
    /// </summary>
    public class Catalog<T> where T : Product
    {
        private readonly List<T> _items = new();

        public void Add(T item)
        {
            if (item == null)
                throw new ArgumentNullException(nameof(item));
            _items.Add(item);
        }

        public void AddRange(IEnumerable<T> items)
        {
            if (items == null)
                throw new ArgumentNullException(nameof(items));
            _items.AddRange(items);
        }

        public void Remove(T item)
        {
            if (item == null) return;
            _items.Remove(item);
        }

        public void RemoveById(int id)
        {
            _items.RemoveAll(i => i.Id == id);
        }

        public void SortByPrice()
        {
            _items.Sort((a, b) => a.GetPrice().CompareTo(b.GetPrice()));
        }

        public List<T> FindIf(Func<T, bool> predicate)
        {
            if (predicate == null)
                throw new ArgumentNullException(nameof(predicate));
            return _items.Where(predicate).ToList();
        }

        public List<T> Search(string query)
        {
            if (string.IsNullOrWhiteSpace(query))
                return new List<T>(_items);
            return _items.Where(p => p.Matches(query)).ToList();
        }

        public T GetById(int id)
        {
            return _items.FirstOrDefault(p => p.Id == id);
        }

        public double GetTotalPrice()
        {
            return _items.Sum(p => p.GetPrice());
        }

        public int Count => _items.Count;

        public bool IsEmpty() => !_items.Any();

        public IReadOnlyList<T> GetAll() => _items.AsReadOnly();

        public List<TResult> Transform<TResult>(Func<T, TResult> transform)
        {
            if (transform == null)
                throw new ArgumentNullException(nameof(transform));
            return _items.Select(transform).ToList();
        }

        public override string ToString() =>
            $"Catalog<{typeof(T).Name}> — {Count} items";
    }
}