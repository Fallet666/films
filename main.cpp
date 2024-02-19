#include <iostream>
#include <sqlite3.h>
#include "person.h"
#include "movies.h"

// Функция для вставки данных в таблицу


int main() {
    sqlite3* DB;
    int exit = sqlite3_open("/Users/aleksejkorotkov/CLionProjects/first_sql/example.db", &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return 1;
    }
    person person(DB);
    movies movies(DB);
    movies.select();
    sqlite3_close(DB);

    return 0;
}
