#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "User.h"
#include "Product.h"
#include "Book.h"
#include "EBook.h"
#include "Purchase.h"
#include "Rating.h"
#include "Cart.h"
#include "IPurchasable.h"

std::vector<std::shared_ptr<User>> users;
std::vector<std::shared_ptr<Product>> products;
std::vector<std::shared_ptr<Rating>> ratings;
std::vector<std::unique_ptr<Purchase>> purchases;

std::shared_ptr<User> findUserById(int id) {
    auto it = std::find_if(users.begin(), users.end(),
        [id](const auto& u) { return u && u->getId() == id; });
    return (it != users.end()) ? *it : nullptr;
}

std::shared_ptr<Product> findProductById(int id) {
    auto it = std::find_if(products.begin(), products.end(),
        [id](const auto& p) { return p && p->getId() == id; });
    return (it != products.end()) ? *it : nullptr;
}

void demoVirtualFunctions() {
    std::cout << "\n=== Демонстрация виртуальных функций ===\n";
    auto book = std::make_shared<Book>(101, "Анна Каренина", "Толстой", 800.0, "978-5-17-123456-7");
    auto ebook = std::make_shared<EBook>(102, "Война и Мир", "Толстой", 700.0, "978-5-17-765432-1", "EPUB");
    std::shared_ptr<Product> p1 = book;
    std::shared_ptr<Product> p2 = ebook;
    std::cout << "p1->toDisplayString(): " << p1->toDisplayString() << "\n";
    std::cout << "p2->toDisplayString(): " << p2->toDisplayString() << "\n";
    std::cout << "Скидка 20% на книгу: " << p1->applyDiscount(20) << "₽\n";
    std::cout << "Скидка 20% на e-книгу: " << p2->applyDiscount(20) << "₽\n";
}

void demoCloning() {
    std::cout << "\n=== Демонстрация клонирования ===\n";
    auto book = std::make_shared<Book>(201, "Мастер и Маргарита", "Булгаков", 900.0, "978-5-990-00000-0");
    auto clonedBook = book->clone();
    std::cout << "Оригинал: " << book->toDisplayString() << "\n";
    std::cout << "Клон:     " << clonedBook->toDisplayString() << "\n";
    std::cout << "ID оригинала: " << book->getId() << ", ID клона: " << clonedBook->getId() << "\n";
}

int main() {
    std::cout << "=== Книжный интернет-магазин ===\n";
    int choice;

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Создать пользователя\n";
        std::cout << "2. Создать книгу\n";
        std::cout << "3. Создать электронную книгу\n";
        std::cout << "4. Добавить в корзину\n";
        std::cout << "5. Оформить заказ\n";
        std::cout << "6. Поставить оценку\n";
        std::cout << "7. Показать данные\n";
        std::cout << "8. Демо: виртуальные функции\n";
        std::cout << "9. Демо: клонирование\n";
        std::cout << "0. Выйти\n";
        std::cout << "Выбор: ";
        std::cin >> choice;

        try {
            if (choice == 1) {
                int id, age;
                std::string name, email, pass;

                std::cout << "ID: ";
                std::cin >> id;
                std::cin.ignore();

                std::cout << "Имя: ";
                std::getline(std::cin, name);

                std::cout << "Email: ";
                std::getline(std::cin, email);

                std::cout << "Возраст: ";
                std::cin >> age;
                std::cin.ignore();

                std::cout << "Хэш пароля: ";
                std::getline(std::cin, pass);

                auto u = std::make_shared<User>(id, name, email, age, pass);
                users.push_back(u);
                std::cout << "Пользователь '" << name << "' создан.\n";
            }
            else if (choice == 2) {
                int id;
                std::string name, author, isbn;
                double price;

                std::cout << "ID: ";
                std::cin >> id;
                std::cin.ignore();

                std::cout << "Название: ";
                std::getline(std::cin, name);

                std::cout << "Автор: ";
                std::getline(std::cin, author);

                std::cout << "ISBN: ";
                std::getline(std::cin, isbn);

                std::cout << "Цена: ";
                std::cin >> price;

                auto p = std::make_shared<Book>(id, name, author, price, isbn);
                products.push_back(p);
                std::cout << "Книга '" << name << "' создана.\n";
            }
            else if (choice == 3) {
                int id;
                std::string name, author, isbn, fmt;
                double price;

                std::cout << "ID: ";
                std::cin >> id;
                std::cin.ignore();

                std::cout << "Название: ";
                std::getline(std::cin, name);

                std::cout << "Автор: ";
                std::getline(std::cin, author);

                std::cout << "ISBN: ";
                std::getline(std::cin, isbn);

                std::cout << "Цена: ";
                std::cin >> price;
                std::cin.ignore();

                std::cout << "Формат (PDF/EPUB): ";
                std::getline(std::cin, fmt);

                if (fmt.empty()) fmt = "PDF";

                auto p = std::make_shared<EBook>(id, name, author, price, isbn, fmt);
                products.push_back(p);
                std::cout << "Электронная книга '" << name << "' создана.\n";
            }
            else if (choice == 4) {
                int uid, pid;

                std::cout << "ID пользователя: ";
                std::cin >> uid;

                std::cout << "ID товара: ";
                std::cin >> pid;

                auto u = findUserById(uid);
                auto p = findProductById(pid);

                if (!u) {
                    std::cout << "Пользователь не найден.\n";
                    continue;
                }
                if (!p) {
                    std::cout << "Товар не найден.\n";
                    continue;
                }

                u->addToCart(p);
                std::cout << "'" << p->getName() << "' → корзина '" << u->getName() << "'.\n";
            }
            else if (choice == 5) {
                int uid;

                std::cout << "ID пользователя: ";
                std::cin >> uid;

                auto u = findUserById(uid);

                if (!u) {
                    std::cout << "Пользователь не найден.\n";
                    continue;
                }

                auto cart = u->getCart();

                if (!cart || !static_cast<bool>(*cart)) {
                    std::cout << "Корзина пуста.\n";
                    continue;
                }

                auto purchase = cart->checkout();
                purchases.push_back(std::move(purchase));
                std::cout << "Заказ #" << purchases.back()->getId()
                    << " на " << purchases.back()->getTotalAmount() << "₽ оформлен.\n";
            }
            else if (choice == 6) {
                int uid, pid, score;

                std::cout << "ID пользователя: ";
                std::cin >> uid;

                std::cout << "ID товара: ";
                std::cin >> pid;

                std::cout << "Оценка (1–5): ";
                std::cin >> score;

                auto u = findUserById(uid);
                auto p = findProductById(pid);

                if (!u) {
                    std::cout << "Пользователь не найден.\n";
                    continue;
                }
                if (!p) {
                    std::cout << "Товар не найден.\n";
                    continue;
                }
                if (score < 1 || score > 5) {
                    std::cout << "Оценка должна быть 1–5.\n";
                    continue;
                }

                auto r = std::make_shared<Rating>(static_cast<int>(ratings.size()) + 1, u, p, score);
                ratings.push_back(r);
                std::cout << "Оценка " << score << " поставлена.\n";
            }
            else if (choice == 7) {
                std::cout << "\n--- ПОЛЬЗОВАТЕЛИ ---\n";
                for (const auto& u : users) {
                    std::cout << "[" << u->getId() << "] " << u->getName()
                        << " <" << u->getEmail() << ">\n";
                }

                std::cout << "\n--- ТОВАРЫ ---\n";
                for (const auto& p : products) {
                    std::cout << *p << "\n";
                }

                std::cout << "\n--- ЗАКАЗЫ ---\n";
                for (const auto& pch : purchases) {
                    std::cout << "Заказ #" << pch->getId()
                        << " — " << pch->getTotalAmount() << "₽\n";
                }

                std::cout << "\n--- ОЦЕНКИ ---\n";
                for (const auto& r : ratings) {
                    std::cout << "Оценка " << r->getScore()
                        << " от " << r->getUser()->getId()
                        << " для " << r->getProduct()->getId() << "\n";
                }
            }
            else if (choice == 8) {
                demoVirtualFunctions();
            }
            else if (choice == 9) {
                demoCloning();
            }
            else if (choice == 0) {
                std::cout << "Выход. До свидания!\n";
                break;
            }
            else {
                std::cout << "Неверный выбор.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}