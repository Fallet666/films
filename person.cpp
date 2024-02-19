//
// Created by Алексей Коротков on 18.02.2024.
//

#include "person.h"
person::person(sqlite3* DB){
    this->DB=DB;
}
void person::insert(int id, const std::string& name, const std::string& date, bool ad) {
    std::string sql = "INSERT INTO PERSON (ID, NAME, DATE, AD) "
                      "VALUES (?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Привязываем параметры к запросу
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, ad ? 1 : 0);

    // Выполняем запрос
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Data inserted successfully." << std::endl;
    }

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
}
void person::select() {
    std::string sql = "SELECT * FROM PERSON";
    sqlite3_stmt* stmt;

    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Вывод заголовка таблицы
    fmt::print("{:<5} {:<35} {:<15} {:<5}\n", "ID", "NAME", "DATE", "AD");
    fmt::print("{:-<64}\n", "");

    // Вывод данных
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* date = sqlite3_column_text(stmt, 2);
        bool ad = sqlite3_column_int(stmt, 3);

        fmt::print("{:<5} {:<35} {:<15} {:<5}\n", id, std::string(reinterpret_cast<const char*>(name)), std::string(reinterpret_cast<const char*>(date)), ad ? "true" : "false");
    }

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
}

int person::getLastID() {
    std::string sql = "SELECT MAX(ID) FROM PERSON";
    sqlite3_stmt* stmt;
    int lastID = -1;

    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }

    // Выполнение запроса и получение результата
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        lastID = sqlite3_column_int(stmt, 0);
    }

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);

    return lastID;
}
// Функция для нормализации ID в таблице
void person::normalizeIds() {
    std::string sql = "UPDATE PERSON SET ID = (SELECT ROW_NUMBER() OVER (ORDER BY ID) FROM PERSON)";
    sqlite3_stmt* stmt;
    // Подготовка запроса
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }
    // Выполнение запроса
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error normalizing IDs: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "IDs normalized successfully." << std::endl;
    }
    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
}

// Функция для удаления элемента из таблицы по ID
void person::deleteById(int id) {
    std::string sql = "DELETE FROM PERSON WHERE ID = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error deleting data: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Data with ID " << id << " deleted successfully." << std::endl;
    }
    std::cout<<"выполнить упорядочивание по id? (1/0)";
    bool norm;
    std::cin>>norm;
    if(norm) normalizeIds();
    sqlite3_finalize(stmt);
}

void person::consoleInsert(){

    std::cout<<"Сколько полей вы хотите добавить? ";
    int n, last=getLastID();
    std::cin>>n;

    bool ad;
    std::string name;
    std::string date;

    for(int i=0;i<n;i++){
        std::cin.ignore();
        std::cout<<"Введите ФИО:";
        getline(std::cin,name);
        std::cout<<"Введите Дату (YYYY-MM-DD):";
        std::cin>>date;
        std::cout<<"Введитe актер/режиссер (1/0)";
        std::cin>>ad;

        insert(i+last+1,name,date,ad);
    }
}

void person::randInsert(int n) {
    std::string fir[10]={"Тарковский ","Сигарев ","Сакуров ","Аксенов ","Балабанов ","Семенов ","Смиронов ","Краус ","Сидоров ","Петров "};
    std::string sec[10]={"Алексадр ","Илья ","Булат ","Тимур ","Саймон ","Жека ","Олег ","Вася ","Иван ","Тимофей "};
    std::string thi[10]={"Александрович","Иванович","Булатович","Тимурович","Саймонович","Арсеньевич","Андреевич","Семенович","Викторович","Евгеньевич"};
    int buf=getLastID()+1;
    for(int i;i<n;i++){
        insert(buf+i,fir[random()%10]+sec[random()%10]+thi[random()%10],std::to_string(random()%100+1900)+"-"+std::to_string(random()%12)+"-"+std::to_string(random()%20),random()%2);
    }
}