using System;
using System.Collections.Generic;
using System.Linq;
using BookRecommender.Models;
using BookRecommender.Services;
using BookRecommender.Interfaces;

namespace BookRecommender
{
    class Program
    {
        // Глобальные контейнеры
        static readonly List<User> Users = new();
        static readonly List<Product> Products = new();
        static readonly List<Rating> Ratings = new();
        static readonly List<Purchase> Purchases = new();

        static User currentUser = null;

        static void Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            Utils.EnsureDataDir();

            Console.WriteLine("=== BookRecommender - Система рекомендаций книг ===\n");

            // Демо данные
            InitializeDemoData();

            while (true)
            {
                try
                {
                    if (currentUser == null)
                    {
                        ShowAuthMenu();
                    }
                    else
                    {
                        ShowMainMenu();
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"\n❌ Ошибка: {ex.Message}\n");
                }
            }
        }

        static void InitializeDemoData()
        {
            // Демо пользователи
            Users.Add(new User(1, "Администратор", "admin@bookstore.ru", 30,
                Utils.SimpleHash("admin123"), UserRole.Admin));
            Users.Add(new User(2, "Иван Петров", "ivan@mail.ru", 25,
                Utils.SimpleHash("user123"), UserRole.Buyer));
            Users.Add(new User(3, "Мария Сидорова", "maria@mail.ru", 28,
                Utils.SimpleHash("user456"), UserRole.Buyer));

            // Демо книги
            Products.Add(new Book(1, "Война и Мир", "Л.Н. Толстой", 1200.0, "978-5-17-123456-7"));
            Products.Add(new Book(2, "Анна Каренина", "Л.Н. Толстой", 900.0, "978-5-17-234567-8"));
            Products.Add(new Book(3, "Преступление и наказание", "Ф.М. Достоевский", 850.0, "978-5-17-345678-9"));
            Products.Add(new EBook(4, "1984", "Дж. Оруэлл", 500.0, "978-5-17-456789-0", "EPUB", 2.5));
            Products.Add(new EBook(5, "Мастер и Маргарита", "М.А. Булгаков", 450.0, "978-5-17-567890-1", "PDF", 3.0));

            Console.WriteLine("✅ Демо данные загружены\n");
        }

        static void ShowAuthMenu()
        {
            Console.WriteLine("\n=== Авторизация ===");
            Console.WriteLine("1. Войти");
            Console.WriteLine("2. Регистрация");
            Console.WriteLine("0. Выход");
            Console.Write("Выбор: ");

            var choice = Console.ReadLine();

            switch (choice)
            {
                case "1":
                    Login();
                    break;
                case "2":
                    Register();
                    break;
                case "0":
                    Console.WriteLine("\n👋 До свидания!");
                    Environment.Exit(0);
                    break;
                default:
                    Console.WriteLine("❌ Неверный выбор");
                    break;
            }
        }

        static void Login()
        {
            Console.WriteLine("\n--- Вход в систему ---");
            int id = Utils.ReadInt("ID пользователя: ", 1);

            var user = Users.FirstOrDefault(u => u.Id == id);
            if (user == null)
            {
                Console.WriteLine("❌ Пользователь не найден");
                return;
            }

            Console.Write("Пароль: ");
            string password = Utils.ReadLineSafe();
            string hash = Utils.SimpleHash(password);

            if (user.PasswordHash == hash)
            {
                currentUser = user;
                Console.WriteLine($"\n✅ Добро пожаловать, {user.Name}!");
            }
            else
            {
                Console.WriteLine("❌ Неверный пароль");
            }
        }

        static void Register()
        {
            Console.WriteLine("\n--- Регистрация ---");
            int id = Users.Max(u => u.Id) + 1;

            Console.Write("Имя: ");
            string name = Utils.ReadLineSafe();
            if (string.IsNullOrWhiteSpace(name))
            {
                Console.WriteLine("❌ Имя не может быть пустым");
                return;
            }

            Console.Write("Email: ");
            string email = Utils.ReadLineSafe();
            if (string.IsNullOrWhiteSpace(email))
            {
                Console.WriteLine("❌ Email не может быть пустым");
                return;
            }

            int age = Utils.ReadInt("Возраст: ", 0, 150);

            Console.Write("Пароль: ");
            string password = Utils.ReadLineSafe();
            if (string.IsNullOrWhiteSpace(password))
            {
                Console.WriteLine("❌ Пароль не может быть пустым");
                return;
            }

            var user = new User(id, name, email, age, Utils.SimpleHash(password));
            Users.Add(user);
            currentUser = user;

            Console.WriteLine($"\n✅ Пользователь '{name}' зарегистрирован. ID={id}");
        }

        static void ShowMainMenu()
        {
            Console.WriteLine($"\n=== Меню ({currentUser.Name}, {currentUser.Role}) ===");

            if (currentUser.Role == UserRole.Admin)
            {
                ShowAdminMenu();
            }
            else
            {
                ShowBuyerMenu();
            }

            Console.WriteLine("\n9. Выйти из аккаунта");
            Console.WriteLine("0. Завершить программу");
            Console.Write("Выбор: ");

            var choice = Console.ReadLine();

            switch (choice)
            {
                case "9":
                    currentUser = null;
                    Console.WriteLine("\n✅ Выход из аккаунта выполнен");
                    break;
                case "0":
                    Console.WriteLine("\n👋 До свидания!");
                    Environment.Exit(0);
                    break;
            }
        }

        static void ShowAdminMenu()
        {
            Console.WriteLine("1. Добавить книгу");
            Console.WriteLine("2. Добавить электронную книгу");
            Console.WriteLine("3. Показать все товары");
            Console.WriteLine("4. Показать всех пользователей");
            Console.WriteLine("5. Отчёт по продажам");
            Console.Write("Выбор: ");

            var choice = Console.ReadLine();

            switch (choice)
            {
                case "1":
                    AddBook();
                    break;
                case "2":
                    AddEBook();
                    break;
                case "3":
                    ShowAllProducts();
                    break;
                case "4":
                    ShowAllUsers();
                    break;
                case "5":
                    ShowSalesReport();
                    break;
                default:
                    Console.WriteLine("❌ Неверный выбор");
                    break;
            }
        }

        static void ShowBuyerMenu()
        {
            Console.WriteLine("1. Каталог книг");
            Console.WriteLine("2. Мои рекомендации");
            Console.WriteLine("3. Корзина");
            Console.WriteLine("4. Оформить заказ");
            Console.WriteLine("5. История покупок");
            Console.WriteLine("6. Оценить товар");
            Console.WriteLine("7. Поиск книг");
            Console.Write("Выбор: ");

            var choice = Console.ReadLine();

            switch (choice)
            {
                case "1":
                    ShowCatalog();
                    break;
                case "2":
                    ShowRecommendations();
                    break;
                case "3":
                    ShowCart();
                    break;
                case "4":
                    Checkout();
                    break;
                case "5":
                    ShowPurchaseHistory();
                    break;
                case "6":
                    RateProduct();
                    break;
                case "7":
                    SearchProducts();
                    break;
                default:
                    Console.WriteLine("❌ Неверный выбор");
                    break;
            }
        }

        static void AddBook()
        {
            Console.WriteLine("\n--- Добавление книги ---");
            int id = Products.Max(p => p.Id) + 1;

            Console.Write("Название: ");
            string name = Utils.ReadLineSafe();

            Console.Write("Автор: ");
            string author = Utils.ReadLineSafe();

            Console.Write("ISBN: ");
            string isbn = Utils.ReadLineSafe();

            double price = Utils.ReadDouble("Цена: ", 0);

            var book = new Book(id, name, author, price, isbn);
            Products.Add(book);

            Console.WriteLine($"\n✅ Книга '{name}' добавлена. ID={id}");
        }

        static void AddEBook()
        {
            Console.WriteLine("\n--- Добавление электронной книги ---");
            int id = Products.Max(p => p.Id) + 1;

            Console.Write("Название: ");
            string name = Utils.ReadLineSafe();

            Console.Write("Автор: ");
            string author = Utils.ReadLineSafe();

            Console.Write("ISBN: ");
            string isbn = Utils.ReadLineSafe();

            double price = Utils.ReadDouble("Цена: ", 0);

            Console.Write("Формат (PDF/EPUB): ");
            string format = Utils.ReadLineSafe();
            if (string.IsNullOrWhiteSpace(format)) format = "PDF";

            double size = Utils.ReadDouble("Размер (МБ): ", 0.1);

            var ebook = new EBook(id, name, author, price, isbn, format, size);
            Products.Add(ebook);

            Console.WriteLine($"\n✅ EBook '{name}' добавлена. ID={id}");
        }

        static void ShowAllProducts()
        {
            Console.WriteLine("\n--- Все товары ---");
            if (!Products.Any())
            {
                Console.WriteLine("Пусто");
                return;
            }

            foreach (var p in Products)
            {
                Console.WriteLine($"  {p}");
            }
        }

        static void ShowAllUsers()
        {
            Console.WriteLine("\n--- Все пользователи ---");
            if (!Users.Any())
            {
                Console.WriteLine("Пусто");
                return;
            }

            foreach (var u in Users)
            {
                Console.WriteLine($"  {u}");
            }
        }

        static void ShowSalesReport()
        {
            Console.WriteLine("\n--- Отчёт по продажам ---");
            if (!Purchases.Any())
            {
                Console.WriteLine("Нет продаж");
                return;
            }

            double total = Purchases.Sum(p => p.TotalAmount);
            Console.WriteLine($"Всего заказов: {Purchases.Count}");
            Console.WriteLine($"Общая сумма: {total:F2}₽");
            Console.WriteLine($"Средний чек: {(total / Purchases.Count):F2}₽");
        }

        static void ShowCatalog()
        {
            Console.WriteLine("\n--- Каталог книг ---");
            if (!Products.Any())
            {
                Console.WriteLine("Пусто");
                return;
            }

            foreach (var p in Products)
            {
                Console.WriteLine($"  {p.ToDisplayString()}");
            }
        }

        static void ShowRecommendations()
        {
            Console.WriteLine("\n--- Персональные рекомендации ---");

            var engine = new RecommendationEngine(Products, Ratings, Users);
            var recs = engine.GetHybridRecommendations(currentUser, 5);

            if (!recs.Any())
            {
                Console.WriteLine("Пока нет рекомендаций. Совершите покупки или поставьте оценки!");
                return;
            }

            int i = 1;
            foreach (var p in recs)
            {
                Console.WriteLine($"  {i++}. {p.ToDisplayString()}");
            }
        }

        static void ShowCart()
        {
            Console.WriteLine("\n--- Корзина ---");
            var cart = currentUser.Cart;

            if (cart.IsEmpty())
            {
                Console.WriteLine("Пуста");
                return;
            }

            foreach (var item in cart.Items)
            {
                Console.WriteLine($"  {item.Product.Name} x{item.Quantity} — {item.Product.GetPrice() * item.Quantity:F2}₽");
            }
            Console.WriteLine($"\nИтого: {cart.GetTotalPrice():F2}₽");
        }

        static void Checkout()
        {
            Console.WriteLine("\n--- Оформление заказа ---");
            var cart = currentUser.Cart;

            if (cart.IsEmpty())
            {
                Console.WriteLine("❌ Корзина пуста");
                return;
            }

            try
            {
                var purchase = cart.Checkout();
                Purchases.Add(purchase);
                currentUser.AddPurchase(purchase);

                Console.WriteLine($"\n✅ Заказ #{purchase.Id} оформлен на сумму {purchase.TotalAmount:F2}₽");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"❌ Ошибка: {ex.Message}");
            }
        }

        static void ShowPurchaseHistory()
        {
            Console.WriteLine("\n--- История покупок ---");
            if (!currentUser.PurchaseHistory.Any())
            {
                Console.WriteLine("Нет покупок");
                return;
            }

            foreach (var p in currentUser.PurchaseHistory)
            {
                Console.WriteLine($"  {p}");
            }
        }

        static void RateProduct()
        {
            Console.WriteLine("\n--- Оценка товара ---");
            ShowCatalog();

            int productId = Utils.ReadInt("ID товара: ", 1);
            var product = Products.FirstOrDefault(p => p.Id == productId);

            if (product == null)
            {
                Console.WriteLine("❌ Товар не найден");
                return;
            }

            int score = Utils.ReadInt("Оценка (1-5): ", 1, 5);

            try
            {
                currentUser.RateProduct(product, score);
                var rating = new Rating(currentUser, product, score);
                Ratings.Add(rating);
                Console.WriteLine("✅ Оценка поставлена");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"❌ Ошибка: {ex.Message}");
            }
        }

        static void SearchProducts()
        {
            Console.WriteLine("\n--- Поиск книг ---");
            Console.Write("Запрос: ");
            string query = Utils.ReadLineSafe();

            var catalog = new Catalog<Product>();
            foreach (var p in Products)
            {
                catalog.Add(p);
            }

            var results = catalog.Search(query);

            if (!results.Any())
            {
                Console.WriteLine("Ничего не найдено");
                return;
            }

            Console.WriteLine($"\nНайдено {results.Count} товаров:");
            foreach (var p in results)
            {
                Console.WriteLine($"  {p.ToDisplayString()}");
            }
        }
    }
}