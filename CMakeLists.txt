cmake_minimum_required(VERSION 3.28)
project(first_sql)

set(CMAKE_CXX_STANDARD 17)

add_executable(first_sql main.cpp
        person.cpp
        person.h
        movies.cpp
        movies.h
        Database.cpp
        Database.h)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(first_sql PRIVATE sqlite3)
target_link_libraries(first_sql PRIVATE fmt::fmt)