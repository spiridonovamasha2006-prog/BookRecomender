using System;
using System.Collections.Generic;

namespace BookRecommender.Models
{
    /// <summary>
    /// Класс пользователя системы
    /// </summary>
    public class User
    {
        public int Id { get; private set; }
        public string Name { get; private set; }
        public string Email { get; private set; }
        public int Age { get; private set; }
        public string PasswordHash { get; private set; }
        public UserRole Role { get; private set; }

        public Cart Cart { get; private set; }
        private readonly List<Purchase> _purchaseHistory = new();
        public IReadOnlyList<Purchase> PurchaseHistory => _purchaseHistory.AsReadOnly();

        private static int _nextId = 1;

        public User()
        {
            Id = _nextId++;
            Role = UserRole.Buyer;
        }

        public User(int id, string name, string email, int age, string passwordHash,
                   UserRole role = UserRole.Buyer)
        {
            if (string.IsNullOrWhiteSpace(name))
                throw new ArgumentException("Name cannot be empty", nameof(name));
            if (string.IsNullOrWhiteSpace(email))
                throw new ArgumentException("Email cannot be empty", nameof(email));
            if (string.IsNullOrWhiteSpace(passwordHash))
                throw new ArgumentException("Password cannot be empty", nameof(passwordHash));
            if (age < 0)
                throw new ArgumentOutOfRangeException(nameof(age), "Age cannot be negative");

            Id = id;
            Name = name;
            Email = email;
            Age = age;
            PasswordHash = passwordHash;
            Role = role;
            Cart = new Cart(this);
        }

        public User(string name, string email, int age, string passwordHash,
                   UserRole role = UserRole.Buyer)
        {
            if (string.IsNullOrWhiteSpace(name))
                throw new ArgumentException("Name cannot be empty", nameof(name));
            if (string.IsNullOrWhiteSpace(email))
                throw new ArgumentException("Email cannot be empty", nameof(email));
            if (string.IsNullOrWhiteSpace(passwordHash))
                throw new ArgumentException("Password cannot be empty", nameof(passwordHash));
            if (age < 0)
                throw new ArgumentOutOfRangeException(nameof(age), "Age cannot be negative");

            Id = _nextId++;
            Name = name;
            Email = email;
            Age = age;
            PasswordHash = passwordHash;
            Role = role;
            Cart = new Cart(this);
        }

        public void AddToCart(Product product)
        {
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            Cart.AddItem(product, 1);
        }

        public void RateProduct(Product product, int rating)
        {
            if (product == null)
                throw new ArgumentNullException(nameof(product));
            if (rating < 1 || rating > 5)
                throw new ArgumentOutOfRangeException(nameof(rating), "Rating must be 1-5");

            Console.WriteLine($"{this.Name} (ID={this.Id}) rated '{product.Name}' with {rating}");
            if (product is IRatable ratable)
                ratable.AddRating(rating);
        }

        public void AddPurchase(Purchase purchase)
        {
            if (purchase == null)
                throw new ArgumentNullException(nameof(purchase));
            _purchaseHistory.Add(purchase);
        }

        public static void ResetIdCounter(int start = 1) => _nextId = start;

        public override string ToString() => $"[User {Id}] {Name} <{Email}> ({Role})";
    }

    public enum UserRole
    {
        Buyer,
        Admin
    }
}