//
// Created by Алексей Коротков on 19.02.2024.
//

#ifndef FIRST_SQL_INTERFACE_H
#define FIRST_SQL_INTERFACE_H

#include <sqlite3.h>
#include <iostream>

#include "Database.h"
#include "movies.h"
#include "person.h"

class interface {
public:
    explicit interface(sqlite3* DB);
    ~interface();
    void changer();

private:
    sqlite3* DB;
    void database();
    void movies();
    void person();
};


#endif //FIRST_SQL_INTERFACE_H
