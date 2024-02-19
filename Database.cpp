
#include <sstream>
#include "Database.h"


    Database::Database(sqlite3* DB) {
        this->DB = DB;
    }

    void Database::findRecentMovies() {
        std::string query = "SELECT title FROM MOVIES WHERE release_date >= date('now', '-1 year')";
        executeAndPrintQuery(query);
    }

void Database::findActorsInMovie(int movieID) {
    // Подготовка SQL-запроса для получения строки с ID актеров для указанного фильма
    std::string sql = "SELECT ACTORS FROM MOVIES WHERE ID = ?";

    // Подготовка оператора запроса
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Привязка параметра (ID фильма) к запросу
    sqlite3_bind_int(stmt, 1, movieID);

    // Выполнение запроса и получение строки с ID актеров
    std::string actorsString;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* actors = sqlite3_column_text(stmt, 0);
        actorsString = reinterpret_cast<const char*>(actors);
    }

    // Освобождение ресурсов
    sqlite3_finalize(stmt);

    // Разделение строки с ID актеров на отдельные ID
    std::istringstream iss(actorsString);
    std::vector<std::string> actorIDs(std::istream_iterator<std::string>{iss},
                                      std::istream_iterator<std::string>());

    // Вывод информации о каждом актере
    std::cout << "Actors in movie with ID " << movieID << ":" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for (const std::string& actorID : actorIDs) {
        // Подготовка SQL-запроса для получения информации об актере по его ID
        std::string actorInfoSQL = "SELECT NAME, AD FROM PERSON WHERE ID = ?";
        sqlite3_stmt* actorStmt;
        if (sqlite3_prepare_v2(DB, actorInfoSQL.c_str(), -1, &actorStmt, NULL) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
            continue;
        }

        // Привязка параметра (ID актера) к запросу
        int actorIDInt = std::stoi(actorID);
        sqlite3_bind_int(actorStmt, 1, actorIDInt);

        // Выполнение запроса и вывод информации об актере
        if (sqlite3_step(actorStmt) == SQLITE_ROW) {
            const unsigned char* name = sqlite3_column_text(actorStmt, 0);
            bool isActor = sqlite3_column_int(actorStmt, 1);

            std::cout << "Name: " << name << " | Role: " << (isActor ? "Actor" : "Director") << std::endl;
        }

        // Освобождение ресурсов
        sqlite3_finalize(actorStmt);
    }
}



void Database::findActorsInNMovies(int nMovies) {
    // Подготовка SQL-запроса для получения всех записей из таблицы MOVIES
    std::string sql = "SELECT ACTORS FROM MOVIES";

    // Подготовка оператора запроса
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Создание словаря для подсчета количества фильмов для каждого актера
    std::unordered_map<int, int> actorMovieCounts;

    // Перебор результатов запроса
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Получение строки с ID актеров для текущего фильма
        const unsigned char* actors = sqlite3_column_text(stmt, 0);
        std::string actorsString = reinterpret_cast<const char*>(actors);

        // Разделение строки с ID актеров на отдельные ID
        std::istringstream iss(actorsString);
        std::vector<std::string> actorIDs(std::istream_iterator<std::string>{iss},
                                          std::istream_iterator<std::string>());

        // Обновление количества фильмов для каждого актера
        for (const std::string& actorID : actorIDs) {
            int actorIDInt = std::stoi(actorID);
            actorMovieCounts[actorIDInt]++;
        }
    }

    // Освобождение ресурсов
    sqlite3_finalize(stmt);

    // Вывод информации об актерах, снявшихся в заданном количестве фильмов
    std::cout << "Actors who appeared in at least " << nMovies << " movies:" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    for (const auto& entry : actorMovieCounts) {
        int actorID = entry.first;
        int movieCount = entry.second;

        if (movieCount >= nMovies) {
            // Подготовка SQL-запроса для получения информации об актере по его ID
            std::string actorInfoSQL = "SELECT NAME FROM PERSON WHERE ID = ?";
            sqlite3_stmt* actorStmt;
            if (sqlite3_prepare_v2(DB, actorInfoSQL.c_str(), -1, &actorStmt, NULL) != SQLITE_OK) {
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
                continue;
            }

            // Привязка параметра (ID актера) к запросу
            sqlite3_bind_int(actorStmt, 1, actorID);

            // Выполнение запроса и вывод информации об актере
            if (sqlite3_step(actorStmt) == SQLITE_ROW) {
                const unsigned char* name = sqlite3_column_text(actorStmt, 0);
                std::cout << "Actor ID: " << actorID << " | Name: " << name << " | Movies: " << movieCount << std::endl;
            }

            // Освобождение ресурсов
            sqlite3_finalize(actorStmt);
        }
    }
}

    void Database::deleteOldMovies(int years) {
        std::string query = "DELETE FROM movies WHERE release_date < date('now', '-" + std::to_string(years) + " years')";
        int result = sqlite3_exec(DB, query.c_str(), NULL, NULL, NULL);
        if (result != SQLITE_OK) {
            std::cerr << "Error deleting old movies: " << sqlite3_errmsg(DB) << std::endl;
        } else {
            std::cout << "Old movies deleted successfully." << std::endl;
        }
    }

    void Database::executeAndPrintQuery(const std::string& query) {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
            return;
        }

        std::cout << "Results:" << std::endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_text(stmt, 0) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
