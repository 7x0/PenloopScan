//
// Created by payloads on 11/26/17.
//

#ifndef NEWSCANDNSTODB_GLOBAL_H
#define NEWSCANDNSTODB_GLOBAL_H
#include <iostream>
#include <vector>
struct Options{
    std::string dict_path;
    std::string nameServer;
    int threadNum;
    std::string domain;
    std::string output;
    long int start;
    std::string config_file;
    std::string DnsServer;
    bool rebuild;
};
struct GlobalSet{
    std::string mysql_host;
    std::string mysql_user;
    std::string mysql_pass;
    std::string mysql_database;
    std::string mysql_table;
    std::string mysql_port;
    std::string ext_file;
    bool debug;
};
struct globalScanResult{
    std::string domain; // www.ss.com
    std::string IP; // 127.0.0.1
    std::vector<int> Port;//80,443,3389
};
#endif //NEWSCANDNSTODB_GLOBAL_H
