//
// Created by Алексей Коротков on 19.02.2024.
//

#include "interface.h"

using namespace std;
void interface::changer() {
    cout<<"Выберите, какого рода команды вы хотите выполнить. \n1 - Задания, 2 - Работа с БД Актеров, 3 - Работа с БД Фильмов, 4 - Выход\n";
    int buf;
    cin>>buf;
    switch (buf) {
        case 1:
            database();
            break;
        case 2:
            person();
            break;
        case 3:
            movies();
            break;
        case 4:
            cout<<"Пока!";
            break;
        default:
            cout<<"Ошибка! Попробуйте ещё раз)\n";
            changer();
    }
}

void interface::database() {
    cout<<"Выберите задание: \n"
          "1 - Найти все фильмы, вышедшие на экран в текущем и прошлом году.\n"
          "2 - Вывести информацию об актерах, снимавшихся в заданном фильме\n"
          "3 - Вывести информацию об актерах, снимавшихся как минимум в N фильмах.\n"
          "4 - Вывести информацию об актерах, которые были режиссерами хотя бы одного из фильмов.\n"
          "5 - Удалить все фильмы, дата выхода которых была более заданного числа лет назад.\n"
          "6 - Назад\n";
    int buf;
    cin>>buf;
    Database database(DB);
    switch (buf) {
        case 1:
            database.findRecentMovies();
            this->database();
            break;
        case 2:
            int ID;
            cout<<"Введите ID фильма: ";
            cin>>ID;
            database.findActorsInMovie(ID);
            this->database();
            break;
        case 3:
            int N;
            cout<<"Введите N (число фильмов): ";
            cin>>N;
            database.findActorsInNMovies(N);
            this->database();
            break;
        case 4:
            database.findActorDirectors();
            this->database();
            break;
        case 5:
            int Years;
            cout<<"Введите число лет: ";
            cin>>Years;
            database.deleteOldMovies(Years);
            this->database();
            break;
        case 6:
            changer();
            break;
        default:
            cout<<"Error\n";
            this->database();
    }
}

void interface::movies() {
    cout<<"Система управления БД фильмов\n";
    cout<<"Выберите действие: \n"
          "1 - Вывести фильмы.\n"
          "2 - Добавить рандомные фильмы.\n"
          "3 - Добавить фильмы вручную.\n"
          "4 - Общее число фильмов.\n"
          "5 - Удалить фильм по ID.\n"
          "6 - Назад\n";
    class movies movies(DB);
    int buf;
    cin>>buf;
    switch (buf) {
        case 1:
            movies.select();
            this->movies();
            break;
        case 2:
            int n;
            cout<<"Сколько рандомных фмльмов добавить?\n";
            cin>>n;
            movies.randInsert(n);
            this->movies();
            break;
        case 3:
            movies.consoleInsert();
            this->movies();
            break;
        case 4:
            cout<<movies.getLastID()<<endl;
            this->movies();
            break;
        case 5:
            int ID;
            cout<<"Какой фильм удалить (ID)?\n";
            cin>>ID;
            movies.deleteById(ID);
            this->movies();
            break;
        case 6:
            changer();
            break;
        default:
            cout<<"Error\n";
            this->movies();
    }

}

void interface::person() {
    cout<<"Система управления БД актеров\n";
    cout<<"Выберите действие: \n"
          "1 - Вывести актеров и режиссеров.\n"
          "2 - Добавить рандомных актеров и режиссеров.\n"
          "3 - Добавить актеров и режиссеров вручную.\n"
          "4 - Общее число актеров и режиссеров.\n"
          "5 - Удалить актеров и режиссеров по ID.\n"
          "6 - Назад\n";
    class person persons(DB);
    int buf;
    cin>>buf;
    switch (buf) {
        case 1:
            persons.select();
            this->person();
            break;
        case 2:
            int n;
            cout<<"Сколько рандомных актеров/режиссеров добавить?\n";
            cin>>n;
            persons.randInsert(n);
            this->person();
            break;
        case 3:
            persons.consoleInsert();
            this->person();
            break;
        case 4:
            cout<<persons.getLastID()<<endl;
            this->person();
            break;
        case 5:
            int ID;
            cout<<"Какого актера/режиссера удалить (ID)?\n";
            cin>>ID;
            persons.deleteById(ID);
            this->person();
            break;
        case 6:
            changer();
            break;
        default:
            cout<<"Error\n";
            this->person();
    }
}

interface::interface(sqlite3 *DB) {
    this->DB=DB;
}

interface::~interface() {
    sqlite3_close(DB);
}
