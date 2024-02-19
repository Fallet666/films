//
// Created by Алексей Коротков on 19.02.2024.
//

#ifndef FIRST_SQL_DATABASE_H
#define FIRST_SQL_DATABASE_H

#include <iostream>
#include <sqlite3.h>
#include <fmt/core.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>
#include <string>

class Database {
public:
    Database(sqlite3* DB);


    void findRecentMovies();
    void findActorsInMovie(int movieID);
    void findActorsInNMovies(int N);
    void findActorDirectors();
    void deleteOldMovies(int years);

private:
    sqlite3* DB;

    void executeAndPrintQuery(const std::string& query);
};




#endif //FIRST_SQL_DATABASE_H
