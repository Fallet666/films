#include "movies.h"

movies::movies(sqlite3* DB){
    this->DB=DB;
}

void movies::insert(int id, const std::string& title, const std::string& actors, const std::string& release_date, const std::string& country) {
    std::string sql = "INSERT INTO MOVIES (ID, TITLE, ACTORS, RELEASE_DATE, COUNTRY) "
                      "VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, actors.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, release_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, country.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Data inserted successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void movies::select() {
    std::string sql = "SELECT * FROM MOVIES";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    fmt::print("{:<5} {:<35} {:<35} {:<15} {:<15}\n", "ID", "TITLE", "ACTORS", "RELEASE_DATE", "COUNTRY");
    fmt::print("{:-<116}\n", "");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* actors = sqlite3_column_text(stmt, 2);
        const unsigned char* release_date = sqlite3_column_text(stmt, 3);
        const unsigned char* country = sqlite3_column_text(stmt, 4);

        fmt::print("{:<5} {:<35} {:<35} {:<15} {:<15}\n", id, std::string(reinterpret_cast<const char*>(title)), std::string(reinterpret_cast<const char*>(actors)), std::string(reinterpret_cast<const char*>(release_date)), std::string(reinterpret_cast<const char*>(country)));
    }

    sqlite3_finalize(stmt);
}

void movies::deleteById(int id) {
    std::string sql = "DELETE FROM MOVIES WHERE ID = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting data: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Data with ID " << id << " deleted successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void movies::normalizeIds() {
    std::string sql = "UPDATE MOVIES SET ID = (SELECT ROW_NUMBER() OVER (ORDER BY ID) FROM MOVIES)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error normalizing IDs: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "IDs normalized successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
}

void movies::consoleInsert() {
    std::cout << "How many records do you want to add? ";
    int n, last = getLastID();
    std::cin >> n;

    std::cin.ignore();
    for (int i = 0; i < n; i++) {
        std::string title, actors, release_date, country;
        std::cout << "Enter title: ";
        std::getline(std::cin, title);
        std::cout << "Enter actors (separated by space): ";
        std::getline(std::cin, actors);
        std::cout << "Enter release date (YYYY-MM-DD): ";
        std::getline(std::cin, release_date);
        std::cout << "Enter country: ";
        std::getline(std::cin, country);

        insert(last + i + 1, title, actors, release_date, country);
    }
}

void movies::randInsert(int n) {
    srand(time(nullptr));
    srandom(time(nullptr));
    std::string titles[10] = {"Побег из Шоушенка", "Зеленая миля", "Форрест Гамп", "Начало", "Крестный отец", "Матрица", "Побег из Алькатраса", "Гладиатор", "Интерстеллар", "Престиж"};
    std::string countries[5] = {"Россия", "США", "Франция", "Великобритания", "Испания"};

    int buf = getLastID() + 1;
    for (int i = 0; i < n; i++) {
        std::string title = titles[random() % 10];
        std::string actors = std::to_string(random() % 10 + 1);

        insert(buf + i, title, actors, "2024-02-18", countries[random() % 5]);
    }
}

int movies::getLastID() {
    std::string sql = "SELECT MAX(ID) FROM MOVIES";
    sqlite3_stmt* stmt;
    int lastID = -1;

    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }

    // Выполнение запроса и получение результата
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        lastID = sqlite3_column_int(stmt, 0);
    }

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);

    return lastID;
}
