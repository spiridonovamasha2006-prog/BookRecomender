#include "Product.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

int& Product::getNextIdRef() {
    static int nextId = 1;
    return nextId;
}

Product::Product()
    : id_(getNextIdRef()++), price_(0.0) {
}

Product::Product(int id, const std::string& name, const std::string& description,
    double price, const std::string& category)
    : id_(id), name_(name), description_(description), price_(price), category_(category) {
}

int Product::getId() const { return id_; }
const std::string& Product::getName() const { return name_; }
const std::string& Product::getDescription() const { return description_; }
const std::string& Product::getCategory() const { return category_; }
const std::vector<std::string>& Product::getTags() const { return tags_; }

Product& Product::setName(const std::string& name) {
    name_ = name;
    return *this;
}

Product& Product::setDescription(const std::string& desc) {
    description_ = desc;
    return *this;
}

Product& Product::setPrice(double price) {
    price_ = price;
    return *this;
}

Product& Product::setCategory(const std::string& cat) {
    category_ = cat;
    return *this;
}

void Product::addTag(const std::string& tag) {
    if (tag.empty()) return;
    if (std::find(tags_.begin(), tags_.end(), tag) == tags_.end()) {
        tags_.push_back(tag);
    }
}

bool Product::containsTag(const std::string& tag) const {
    return std::find(tags_.begin(), tags_.end(), tag) != tags_.end();
}

double Product::getPrice() const {
    return price_;
}

std::string Product::toDisplayString() const {
    std::ostringstream oss;
    oss << "«" << name_ << "» (" << category_ << ") — "
        << std::fixed << std::setprecision(2) << price_ << "₽";
    return oss.str();
}

bool Product::operator==(const Product& other) const {
    return id_ == other.id_;
}

bool Product::operator!=(const Product& other) const {
    return !(*this == other);
}

bool Product::operator<(const Product& other) const {
    return price_ < other.price_;
}

std::ostream& operator<<(std::ostream& os, const Product& p) {
    os << "[" << p.id_ << "] " << p.name_ << " — " << p.price_ << "₽";
    return os;
}