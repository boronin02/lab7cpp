#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm> // Для std::sort и std::find_if
#include <stdexcept>

using namespace std;

// Абстрактный класс для пользователей
class AbstractUser {
public:
    virtual void displayInfo() const = 0; // Чисто виртуальная функция
    virtual ~AbstractUser() = default;
};

// Базовый класс User
class User : public AbstractUser {
protected:
    int id;                  // ID пользователя
    std::string name;        // Имя пользователя
    std::string username;    // Логин
    double balance;          // Баланс
    static int userCount;    // Статическое поле: количество пользователей

public:
    User(int userId, const std::string& userName, const std::string& userUsername, double userBalance)
        : id(userId), name(userName), username(userUsername), balance(userBalance) {
        ++userCount;
    }

    virtual void displayInfo() const override {
        cout << "ID: " << id << ", Имя: " << name << ", Логин: " << username
            << ", Баланс: " << balance << " руб.\n";
    }

    virtual ~User() { --userCount; }

    double getBalance() const {
        return balance;
    }

    void addBalance(double amount) {
        balance += amount;
    }

    int getId() const {
        return id;
    }

    static int getUserCount() {
        return userCount;
    }
};

int User::userCount = 0;

// Производный класс PremiumUser
class PremiumUser : public User {
private:
    double bonusBalance;

public:
    PremiumUser(int userId, const std::string& userName, const std::string& userUsername, double userBalance, double bonus)
        : User(userId, userName, userUsername, userBalance), bonusBalance(bonus) {}

    void displayInfo() const override {
        User::displayInfo();
        cout << "Бонусный баланс: " << bonusBalance << " руб.\n";
    }

    void useBonus(double amount) {
        if (bonusBalance < amount) throw std::logic_error("Недостаточно бонусов!");
        bonusBalance -= amount;
    }
};

// Производный класс Admin
class Admin : public User {
private:
    std::string accessLevel;

public:
    Admin(int userId, const std::string& userName, const std::string& userUsername, double userBalance, const std::string& access)
        : User(userId, userName, userUsername, userBalance), accessLevel(access) {}

    void displayInfo() const override {
        cout << "[Admin] ";
        User::displayInfo();
        cout << "Уровень доступа: " << accessLevel << endl;
    }
};

// Класс Game
class Game {
private:
    std::string gameName;
    double minBet;
    double maxBet;
    double payoutMultiplier;

public:
    Game(const std::string& name, double min, double max, double multiplier)
        : gameName(name), minBet(min), maxBet(max), payoutMultiplier(multiplier) {}

    const std::string& getGameName() const {
        return gameName;
    }

    double calculatePayout(double betAmount, bool isWin) const {
        return isWin ? betAmount * payoutMultiplier : 0;
    }

    void displayInfo() const {
        cout << "Игра: " << gameName << ", Мин. ставка: " << minBet
            << " руб., Макс. ставка: " << maxBet << " руб., Множитель: " << payoutMultiplier << "x\n";
    }
};

// Функция сортировки пользователей по балансу
void sortUsersByBalance(vector<shared_ptr<User>>& users) {
    sort(users.begin(), users.end(), [](const shared_ptr<User>& a, const shared_ptr<User>& b) {
        return a->getBalance() > b->getBalance(); // Сортировка по убыванию
        });
}

// Функция поиска пользователя по ID
shared_ptr<User> findUserById(const vector<shared_ptr<User>>& users, int id) {
    auto it = find_if(users.begin(), users.end(), [id](const shared_ptr<User>& user) {
        return user->getId() == id;
        });
    if (it != users.end()) {
        return *it;
    }
    else {
        throw runtime_error("Пользователь с таким ID не найден!");
    }
}

int main() {
    setlocale(LC_ALL, "rus");

    // Вектор для хранения указателей на пользователей
    vector<shared_ptr<User>> users;

    // Добавляем пользователей
    users.push_back(make_shared<User>(1, "Иван", "ivan123", 100.0));
    users.push_back(make_shared<PremiumUser>(2, "Мария", "maria456", 300.0, 50.0));
    users.push_back(make_shared<Admin>(0, "Админ", "admin", 1000.0, "Full"));
    users.push_back(make_shared<User>(3, "Алексей", "alex789", 200.0));

    cout << "Список пользователей:\n";
    for (const auto& user : users) {
        user->displayInfo();
    }

    // Сортировка пользователей по балансу
    cout << "\nСортировка пользователей по балансу:\n";
    sortUsersByBalance(users);
    for (const auto& user : users) {
        user->displayInfo();
    }

    // Поиск пользователя по ID с вводом с клавиатуры
    cout << "\nВведите ID пользователя для поиска: ";
    int searchId;
    cin >> searchId;

    try {
        auto foundUser = findUserById(users, searchId);
        cout << "Найденный пользователь:\n";
        foundUser->displayInfo();
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    return 0;
}
