#include <iostream>
#include "vector"
#include <sqlite3.h>
#include "person.h"
#include "movies.h"
#include "Database.h"

// Функция для вставки данных в таблицу


int main() {
    sqlite3* DB;
    int exit = sqlite3_open("/Users/aleksejkorotkov/CLionProjects/first_sql/example.db", &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return 1;
    }
    Database database(DB);
    database.findActorsInNMovies(2);
    sqlite3_close(DB);

    return 0;
}
