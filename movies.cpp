//
// Created by Алексей Коротков on 19.02.2024.
//

#include "movies.h"

movies::movies(sqlite3 *DB) {
    this->DB=DB;
}

void movies::insert(int id, const std::string& name, const std::string& release_date, const std::string& country) {
    std::string sql = "INSERT INTO MOVIES (ID, NAME, RELEASE_DATE, COUNTRY) "
                      "VALUES (?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Привязываем параметры к запросу
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, release_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, country.c_str(), -1, SQLITE_STATIC);

    // Выполняем запрос
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Data inserted successfully." << std::endl;
    }

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
}

void movies::select() {
    std::string sql = "SELECT * FROM MOVIES";
    sqlite3_stmt* stmt;

    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Вывод заголовка таблицы
    fmt::print("{:<5} {:<35} {:<15} {:<15}\n", "ID", "NAME", "RELEASE_DATE", "COUNTRY");
    fmt::print("{:-<70}\n", "");

    // Вывод данных
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* release_date = sqlite3_column_text(stmt, 2);
        const unsigned char* country = sqlite3_column_text(stmt, 3);

        fmt::print("{:<5} {:<35} {:<15} {:<15}\n", id, std::string(reinterpret_cast<const char*>(name)), std::string(reinterpret_cast<const char*>(release_date)), std::string(reinterpret_cast<const char*>(country)));
    }

    // Освобождаем ресурсы
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
    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }
    // Выполнение запроса
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error normalizing IDs: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "IDs normalized successfully." << std::endl;
    }
    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
}
