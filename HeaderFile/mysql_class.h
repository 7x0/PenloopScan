//
// Created by payloads on 11/26/17.
//

#ifndef NEWSCANDNSTODB_MYSQL_CLASS_H
#define NEWSCANDNSTODB_MYSQL_CLASS_H
#include "../HeaderFile/global.h"
#include <mysql/mysql.h>
#include <iostream>
#include <vector>
enum show_type{
    fields,
    data
};
class mysql_class{
public:
    bool mysql_debug=false;
    enum show_type Type;
    bool connect(std::string host,std::string user,std::string pass,std::string database,short int port);
    MYSQL_RES * Select(std::string sql);
    int Update(std::string sql);
    void ShowData(MYSQL_RES * mysqlRes);
    std::string Escape(std::string str);
    ~mysql_class();
    int Insert(std::string tableName,std::vector<std::string>columns,std::vector<std::string>data);
    int real_Insert(std::string tableName,std::vector<std::string>columns,std::vector<std::string>data);
    void printVersionTest();
    bool selectDB(std::string dbname);
private:
    MYSQL_RES * Query(std::string sql);
    int exec(std::string sql);
    MYSQL * sqler;
    MYSQL_FIELD * mysql_field;
    MYSQL_ROW mysqlRow;
};
#endif //NEWSCANDNSTODB_MYSQL_CLASS_H
