using System;
using BookRecommender.Interfaces;

namespace BookRecommender.Models
{
    /// <summary>
    /// Класс печатной книги
    /// </summary>
    public class Book : Product
    {
        public string Author { get; protected set; }
        public string Isbn { get; protected set; }

        public Book() : base() { }

        public Book(int id, string title, string author, double price, string isbn)
            : base(title, $"Книга автора {author}", price, "Книга")
        {
            if (string.IsNullOrWhiteSpace(author))
                throw new ArgumentException("Author cannot be empty", nameof(author));
            if (string.IsNullOrWhiteSpace(isbn))
                throw new ArgumentException("ISBN cannot be empty", nameof(isbn));

            Id = id;
            Author = author;
            Isbn = isbn;
            AddTag("book");
            AddTag("paper");
        }

        public Book(string title, string author, double price, string isbn)
            : base(title, $"Книга автора {author}", price, "Книга")
        {
            if (string.IsNullOrWhiteSpace(author))
                throw new ArgumentException("Author cannot be empty", nameof(author));
            if (string.IsNullOrWhiteSpace(isbn))
                throw new ArgumentException("ISBN cannot be empty", nameof(isbn));

            Author = author;
            Isbn = isbn;
            AddTag("book");
            AddTag("paper");
        }

        public override string ToDisplayString() =>
            $"«{Name}» ({Author}, {Category}) — {Price:F2}₽";

        public override bool Matches(string query)
        {
            if (string.IsNullOrWhiteSpace(query)) return false;
            var q = query.ToLowerInvariant();
            return base.Matches(query) ||
                   Author.ToLowerInvariant().Contains(q);
        }

        public override object Clone() => new Book(Id, Name, Author, Price, Isbn);

        public override string ToString() => $"[Book {Id}] {Name} — {Author} — {Price:F2}₽";
    }
}