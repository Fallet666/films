#include <iostream>
#include "interface.h"
#include <sqlite3.h>


// Функция для вставки данных в таблицу


int main() {
    sqlite3* DB;
    int exit = sqlite3_open("/Users/aleksejkorotkov/CLionProjects/first_sql/example.db", &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return 1;
    }
    interface interface(DB);
    interface.changer();

    return 0;
}
