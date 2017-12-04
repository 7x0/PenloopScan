//
// Created by payloads on 11/26/17.
//
#ifndef NEWSCANDNSTODB_CONTROLLER_CLASS_H
#define NEWSCANDNSTODB_CONTROLLER_CLASS_H
#include "global.h"
#include <iostream>
#include <sys/time.h>
#include <sstream>
#include <getopt.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include "mysql_class.h"
#include "dns_class.h"
#include "sockscan_class.h"
#include "http_class.h"
#include <regex>
#define VERSION 0.11
#define CREATE_TIME 20171123

extern std::vector<struct globalScanResult> GlobalScanData;
extern struct Options * globalOptions;
extern struct GlobalSet * globalSet;
class controller{
public:
    bool start(int argc,char * argv[]);
    void help(char * argv[]);
    void loadConfig();
    bool ConnectMysql();
    bool readDict();
    bool rebulid();
    controller();
    ~controller();
public:
    bool debug;
    std::vector<std::string> targetList;
    mysql_class * Mysql;
    dns_class * Dns;
    sockscan_class * Sock;
    http_class * Http;
private:
    bool readConfigFile(const std::string & key, std::string & value);
    void errorHandle();
private:
    enum {
        CONFIG_CANT_READ,
        MYSQL_CANT_CONNECT,
        MYSQL_SQL_ERROR,
        OPTION_LOST,
        CONFIG_MYSQL_HOST_LOST,
        CONFIG_MYSQL_USER_LOST,
        CONFIG_MYSQL_PASS_LOST,
        CONFIG_MYSQL_TABLE_LOST,
        CONFIG_MYSQL_DATABSE_LOST,
        CONFIG_MYSQL_PORT_LOST,
        MYSQL_TABLE_NOT_CREATED,
        NOT_GIVE_DOMAIN,
        NOT_GIVE_DOMAIN_DICT,
        CANT_READ_DICT,
        PORTS_LOST,
        UNKNOWN_ERROR
    }Error;
};
#endif //NEWSCANDNSTODB_CONTROLLER_CLASS_H
