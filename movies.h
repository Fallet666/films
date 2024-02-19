//
// Created by Алексей Коротков on 19.02.2024.
//

#ifndef FIRST_SQL_MOVIES_H
#define FIRST_SQL_MOVIES_H

#include <iostream>
#include <sqlite3.h>
#include <fmt/core.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

class movies {
private:
    sqlite3* DB;
public:
    movies(sqlite3* DB);

    void insert(int id, const std::string& title, const std::string& actors, const std::string& release_date, const std::string& country);
    void select();
    void deleteById(int id);
    void normalizeIds();

    void consoleInsert();
    void randInsert(int n);

    int getLastID();
};



#endif //FIRST_SQL_MOVIES_H
