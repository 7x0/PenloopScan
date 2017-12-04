//
// Created by payloads on 11/27/17.
//
#ifndef UNTITLED3_SOCKSCAN_CLASS_H
#define UNTITLED3_SOCKSCAN_CLASS_H
#include <vector>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/signal.h>
#include "global.h"
#include "mysql_class.h"
#include <sstream>
#include <pthread.h>
extern std::vector<struct globalScanResult> GlobalScanData;
extern struct Options * globalOptions;
extern struct GlobalSet * globalSet;

class sockscan_class{
public:
  int socket_scan;
  sockscan_class();
  void startScan(int pthreads);
  bool IntoDatabase(mysql_class * Mysql,std::string tablename);
  static void * scan(void * sockscan_class1);
  pthread_mutex_t mtx;
    ~sockscan_class();
};
#endif //UNTITLED3_SOCKSCAN_CLASS_H
