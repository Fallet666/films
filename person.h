//
// Created by Алексей Коротков on 18.02.2024.
//

#ifndef FIRST_SQL_PERSON_H
#define FIRST_SQL_PERSON_H
#include <sqlite3.h>
#include <iostream>
#include <iomanip>
#include <fmt/core.h>
#include <string>
class person {
private:
    sqlite3* DB;
public:
    person(sqlite3* DB);
    int getLastID();
    void normalizeIds();
    void deleteById(int id);
    void insert(int id, const std::string& name, const std::string& date, bool ad);
    void consoleInsert();
    void select();
    void randInsert(int n);
};


#endif //FIRST_SQL_PERSON_H
