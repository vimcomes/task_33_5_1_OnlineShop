#include <iostream>
#include <map>
#include <exception>

class RuntimeError: public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "runtime_error";
    }
};

class Basket {
private:
    std::map<std::string, int> products;
public:
    void add (std::string& inArticle, int inCount) {
        auto it = products.find(inArticle);
        if (it == products.end()) {
            products.insert(std::pair<std::string, int>(inArticle, inCount));
            std::cout << "The product was successfully added to the cart" << std::endl;
        } else {
            it->second += inCount;
            std::cout << "The product was successfully added to the cart" << std::endl;
        }

    }
    int remove (std::string& inArticle, int inCount){
        auto it = products.find(inArticle);
        try {
            if (it == products.end()) {
                throw RuntimeError();
            }
        } catch (const std::exception& x) {
            std::cerr << "There is no such product in the basket: " << x.what() << std::endl;
            return 0;
        }
        if (it->second > inCount) {
            it->second -= inCount;
            std::cout << "The product was successfully removed from the basket" << std::endl;
            return inCount;
        } else {
            int count = it->second;
            products.erase(it);
            std::cout << "The product was successfully removed from the basket" << std::endl;
            return count;
        }

    }
};

class Market {
private:
    Basket cart;
    std::map<std::string, int> products;
public:
    void fillData () {
        std::string article;
        int count;
        std::cout << "Fill the warehouse database with the necessary products" << std::endl;
        while (true) {
            std::cout << R"(Enter product article or "exit" to exit: )";
            std::cin >> article;
            if (article == "exit") {
                break;
            }
            std::cout << "Enter the quantity of the product: ";
            std::cin >> count;
            try {
                if (count < 0) {
                    throw std::invalid_argument("count");
                }
            }
            catch (const std::exception& x) {
                std::cerr << "Invalid argument supplied: " << x.what();
                continue;
            }
            products.insert(std::pair<std::string, int>(article, count));
        }
    }

    void shopping () {
        std::cout << "SHOPPING" << std::endl;
        while (true) {
            std::string article;
            std::string command;
            int count;
            std::cout << R"(Enter product article or "exit" to exit: )";
            std::cin >> article;
            if (article == "exit") {
                break;
            }
            std::cout << "Enter the quantity of the product: ";
            std::cin >> count;
            try {
                if (count < 0) {
                    throw std::invalid_argument("count");
                }
            }
            catch (const std::exception& x) {
                std::cerr << "Invalid argument supplied: " << x.what();
                continue;
            }
            std::cout << R"(Choose command "add" to or "remove" from basket: )";
            std::cin >> command;
            try {
                if (command != "add" && command != "remove") {
                    throw std::invalid_argument("command");
                }
            }
            catch (const std::exception& x) {
                std::cerr << "Invalid argument supplied: " << x.what();
                continue;
            }
            auto it = products.find(article);
            if (command == "add") {
                try {
                    if (it == products.end()) {
                        throw RuntimeError();
                    }
                }
                catch (const std::exception& x) {
                    std::cerr << "Missing product article: " << x.what() << std::endl;
                    continue;
                }
                try {
                    if (it->second < count) {
                        throw std::invalid_argument("count");
                    }
                }
                catch (const std::exception& x) {
                    std::cerr << "Invalid argument supplied: " << x.what() << std::endl;
                    continue;
                }
                it->second -= count;
                cart.add(article, count);
            } else if (command == "remove") {
                it->second += cart.remove(article, count);
            }
        }
    }
};
int main() {
    Market shop;
    shop.fillData();
    shop.shopping();
    return 0;
}
