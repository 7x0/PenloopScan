//
// Created by payloads on 11/26/17.
//
#include "../HeaderFile/mysql_class.h"
bool mysql_class::connect(std::string host, std::string user, std::string pass, std::string database, short int port) {
    sqler = mysql_init(NULL);
    if(!mysql_real_connect(sqler,host.c_str(),user.c_str(),pass.c_str(),database.c_str(),port,NULL,0)){
        std::cout << mysql_error(sqler)<< std::endl;
        mysql_close(sqler);
        exit(-1);
    }
    Type = fields;
    return true;
}

bool mysql_class::selectDB(std::string dbname){
    mysql_select_db(sqler,dbname.c_str());
    std::cout << mysql_error(sqler)<< std::endl;
    return true;
}

int mysql_class::real_Insert(std::string tableName, std::vector<std::string> columns, std::vector<std::string> data) {
    int code = 0;
    if(columns.size() != data.size() && mysql_debug){
        std::cout << "column count number not eq data number" << std::endl;
        return -1;
    }
    std::string sql = "INSERT INTO "+tableName;
    for(int j = 0; j < columns.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (columns.size()-1)){
            sql += columns[j]+")";
            break;
        }
        sql+=columns[j]+",";
    }
    sql+="VALUES";
    for(int j = 0; j < data.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (data.size()-1)){
            sql += data[j]+")";
            break;
        }
        sql+= data[j]+",";
    }
    if((code =mysql_query(sqler,sql.c_str())) == 0 && mysql_debug){
        std::cout << "[Error] :" << mysql_error(sqler) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return -1;
    }
    if(mysql_debug){
        std::cout <<"[+]SQL : " <<sql << std::endl;
        std::cout << "Insert ID:" << mysql_insert_id(sqler) << std::endl;
    }
    return code;
}
int mysql_class::Insert(std::string tableName, std::vector<std::string> columns, std::vector<std::string> data) {
    int code = 0;
    if(columns.size() != data.size() && mysql_debug){
        std::cout << "column count number not eq data number" << std::endl;
        return -1;
    }
    std::string sql = "INSERT INTO "+tableName;
    for(int j = 0; j < columns.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (columns.size()-1)){
            sql += Escape(columns[j])+")";
            break;
        }
        sql+=Escape(columns[j])+",";
    }
    sql+="VALUES";

    for(int j = 0; j < data.size(); ++j) {
        if(j==0){
            sql += " (";
        }
        if(j == (data.size()-1)){
            sql += "'"+Escape(data[j])+"')";
            break;
        }
        sql+= "'"+Escape(data[j])+"',";
    }
    if((code =mysql_query(sqler,sql.c_str())) != 0 && mysql_debug){
        std::cout << "[Error] :" << mysql_error(sqler) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return -1;
    }
    if(mysql_debug){
        std::cout << "Insert ID:" << mysql_insert_id(sqler) << std::endl;
        std::cout <<"[+]SQL : " <<sql << std::endl;
    }

    return code;
}

int mysql_class::exec(std::string sql) {
    int code = 0;
    if((code =mysql_query(sqler,sql.c_str())) != 0 && mysql_debug){
        std::cout << "[Error] :" << mysql_error(sqler) << std::endl;
        std::cout << "[SQL] :" << sql << std::endl;
        return -1;
    }
    return code;
}

MYSQL_RES* mysql_class::Select(std::string sql) {
    return Query(sql);
}

MYSQL_RES* mysql_class::Query(std::string sql) {
    if(mysql_query(sqler,sql.c_str()) != 0 && mysql_debug){
        std::cout << "QUERY : " << sql  << std::endl;
        std::cout << mysql_error(sqler)<< std::endl;
        return NULL;
    }
    return mysql_use_result(sqler);
}

int mysql_class::Update(std::string sql) {
    return exec(sql);
}

void mysql_class::ShowData(MYSQL_RES *mysqlRes) {
    switch (Type){
        case fields:
            for(int i=0;mysql_field=mysql_fetch_field(mysqlRes);i++)//获取列名
            {
                std::cout << mysql_field->name << " \t ";
            }
            std::cout << "\n";
        default:
            while ((mysqlRow = mysql_fetch_row(mysqlRes)) != NULL)
            {
                for(int i=0;i<mysql_num_fields(mysqlRes);i++){
                    std::cout << mysqlRow[i] << " \t";
                }
                std::cout << "\n";
            }
    }
}

std::string mysql_class::Escape(std::string str) {
    char resultSQL[(str.length()*2)+1];
    mysql_real_escape_string(sqler,resultSQL,str.c_str(),str.length());
    std::string realsql = resultSQL;
    return realsql;
}

mysql_class::~mysql_class() {
    mysql_close(sqler);
}