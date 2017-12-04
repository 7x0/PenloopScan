//
// Created by payloads on 11/29/17.
//
#include "../HeaderFile/http_class.h"
#include <pthread.h>
void * http_class::getTitle(void * pVoid) {
    http_class * T = (http_class *)pVoid;
    for (auto i = GlobalScanData.begin(); i != GlobalScanData.end(); ++i) {
        for (auto port = i->Port.begin(); port != i->Port.end(); ++port) {
            std::string url = "";
            std::stringstream P;
            P << *port;
            if(*port == 443){
                url+="https://" + i->domain;
            }else{
                url+="http://"  + i->domain+":"+ P.str();
            }
            P.clear();
            std::cout << "[*]URL:" << url << std::endl;
            CURL * request = curl_easy_init();
            curl_easy_setopt(request,CURLOPT_NOBODY,1);
            curl_easy_setopt(request,CURLOPT_HEADERDATA,stdout);
            curl_easy_setopt(request,CURLOPT_URL,url.c_str());
            curl_easy_setopt(request,CURLOPT_TIMEOUT,1);
            curl_easy_setopt(request,CURLOPT_SSL_VERIFYPEER,false);
            curl_easy_setopt(request,CURLOPT_SSL_VERIFYHOST,false);
            pthread_mutex_lock(&T->mtx);
            CURLcode error = curl_easy_perform(request);
            pthread_mutex_unlock(&T->mtx);
            url.clear();
            long code;
            curl_easy_getinfo(request,CURLINFO_RESPONSE_CODE,&code);
            std::cout << curl_easy_strerror(error) << std::endl;
            std::cout <<  "Code :" <<code <<std::endl;
            curl_easy_cleanup(request);
        }
    }
    return NULL;
}

void http_class::loadthread() {
    pthread_mutex_init(&mtx,NULL);
    for (int i = 0; i < 10; ++i) {
        pthread_create(&p[i],NULL,getTitle,this);
    }
    for (int j = 0; j < 10; ++j) {
        pthread_join(p[j],NULL);
    }
}
