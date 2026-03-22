using System;

namespace BookRecommender.Models
{
    /// <summary>
    /// Класс электронной книги
    /// </summary>
    public class EBook : Book, IDisposable
    {
        public string Format { get; protected set; }
        public double FileSizeMB { get; protected set; }

        private bool _disposed = false;

        public EBook() : base() { }

        public EBook(int id, string title, string author, double price, string isbn,
                    string format = "PDF", double fileSizeMB = 1.0)
            : base(id, title, author, price, isbn)
        {
            if (string.IsNullOrWhiteSpace(format))
                throw new ArgumentException("Format cannot be empty", nameof(format));
            if (fileSizeMB <= 0)
                throw new ArgumentOutOfRangeException(nameof(fileSizeMB), "File size must be positive");

            Format = format;
            FileSizeMB = fileSizeMB;
            Category = "Электронная книга";
            AddTag("ebook");
            AddTag("digital");
        }

        public EBook(string title, string author, double price, string isbn,
                    string format = "PDF", double fileSizeMB = 1.0)
            : base(title, author, price, isbn)
        {
            if (string.IsNullOrWhiteSpace(format))
                throw new ArgumentException("Format cannot be empty", nameof(format));
            if (fileSizeMB <= 0)
                throw new ArgumentOutOfRangeException(nameof(fileSizeMB), "File size must be positive");

            Format = format;
            FileSizeMB = fileSizeMB;
            Category = "Электронная книга";
            AddTag("ebook");
            AddTag("digital");
        }

        public override double GetPrice()
        {
            double basePrice = base.GetPrice();
            return Math.Max(50.0, basePrice * 0.9); // Скидка 10% на цифру
        }

        public override string ToDisplayString() =>
            $"«{Name}» ({Format}, {Category}) — {GetPrice():F2}₽ (электронная)";

        public override double ApplyDiscount(double discountPercent)
        {
            return base.ApplyDiscount(discountPercent + 10); // Доп. скидка 10%
        }

        public override object Clone() => new EBook(Id, Name, Author, Price, Isbn, Format, FileSizeMB);

        public void Dispose()
        {
            if (!_disposed)
            {
                Console.WriteLine($"EBook '{Name}' disposed.");
                _disposed = true;
            }
        }

        ~EBook() => Dispose();

        public override string ToString() => $"[EBook {Id}] {Name} — {Format} — {GetPrice():F2}₽";
    }
}