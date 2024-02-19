#include <iostream>
#include <sqlite3.h>
#include "string"
#include "person.h"

// Функция для вставки данных в таблицу


int main() {
    sqlite3* DB;
    int exit = sqlite3_open("example.db", &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return 1;
    }
    person person(DB);
    person.select();
    sqlite3_close(DB);

    return 0;
}
