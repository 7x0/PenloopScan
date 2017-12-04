//
// Created by payloads on 11/29/17.
//

#ifndef UNTITLED3_HTTP_CLASS_H
#define UNTITLED3_HTTP_CLASS_H
#include "global.h"
#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <pthread.h>

extern std::vector<struct globalScanResult> GlobalScanData;
extern struct Options * globalOptions;
extern struct GlobalSet * globalSet;
class http_class{
public:
    static void * getTitle(void * pVoid);
    void loadthread();
    pthread_t p[10];
    // static int it = 0;
    pthread_mutex_t mtx;
};
#endif //UNTITLED3_HTTP_CLASS_H
