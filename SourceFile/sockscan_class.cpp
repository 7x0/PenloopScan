#include "../HeaderFile/sockscan_class.h"
void * sockscan_class::scan(void * sockscan_class1) {
    sockscan_class * sockscanThis = (sockscan_class *)sockscan_class1;
    //unsigned int ports[] ={80,8080,8082,8081,8089};
    std::vector<unsigned  int> ports ={80,8080,8082,8081,8089,8001,8002,443,7000,7777};
    int j = 0;
    for(auto global_jobs=GlobalScanData.begin();global_jobs!=GlobalScanData.end();global_jobs++){
        for (int i = 0; i < ports.size(); ++i) {
            sockscanThis->socket_scan = socket(AF_INET,SOCK_STREAM,0);
            struct sockaddr_in target;
            target.sin_family =AF_INET;
            target.sin_addr.s_addr = inet_addr(global_jobs->IP.c_str());
            target.sin_port = htons(ports[i]);
            struct timeval timeout ={0,50000};
            setsockopt(sockscanThis->socket_scan,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));
            if(connect(sockscanThis->socket_scan,(struct sockaddr *)&target,sizeof(target)) == 0){
                GlobalScanData[j].Port.push_back((int)ports[i]);
                std::cout << global_jobs->IP <<":"<< ports[i] << " [Open]"<<std::endl;
            }
            close(sockscanThis->socket_scan);
        }
        j++;
    }

    /*
    for(auto it = GlobalScanData.begin();it != GlobalScanData.end();it++){
        pthread_mutex_lock(&sockscanThis->mutex);
        for (int i = 0; i < length; ++i) {
            sockscanThis->socket_scan = socket(AF_INET,SOCK_STREAM,0);
            struct sockaddr_in target;
            // std::cout <<"trying Port ..   " << ports[i] << std::endl;
            target.sin_family =AF_INET;
            target.sin_addr.s_addr = inet_addr(it->IP.c_str());
            target.sin_port = htons(ports[i]);
            struct timeval timeout ={0,10000};
            setsockopt(sockscanThis->socket_scan,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(timeout));
            if(connect(sockscanThis->socket_scan,(struct sockaddr *)&target,sizeof(target)) == 0){
                it->Port.push_back((int)ports[i]);
                std::cout << it->IP <<":"<< ports[i] << " [Open]"<<std::endl;
            }
            close(sockscanThis->socket_scan);
        }
        pthread_mutex_unlock(&sockscanThis->mutex);

    }
     */
    return NULL;
}
bool sockscan_class::IntoDatabase(mysql_class * Mysql,std::string tableName) {
    std::vector<std::string>columns = {"md51","domain","ip","md52"};
    std::vector<std::string> data;
    for (auto i = GlobalScanData.begin(); i != GlobalScanData.end() ; ++i) {
        std::string ports = "";
        for (auto p = i->Port.begin(); p != i->Port.end(); ++p) {
            std::stringstream ss;
            std::string tmp;
            ss << *p;
            ss >> tmp;
            // std::cout << tmp << std::endl;
            if(p == (i->Port.end()-1)){
                ports+=tmp;
                continue;
            }
            ports+=tmp+",";
            tmp.clear();
            ss.clear();
        }
        std::vector<std::string>columns = {"md51","domain","ip","md52","ports"};
        std::string md51 = "md5('" + i->IP + i->domain + ports +"')";
        std::string md52 = "md5('"+i->domain + "')";
        std::string domain = "'"+i->domain+"'";
        std::string IP = "'"+i->IP+"'";
        ports = "'" + ports +"'";
        std::vector<std::string>data = {md51,domain,IP,md52,ports};
        Mysql->real_Insert(tableName,columns,data);
    }
}

sockscan_class::sockscan_class() {
    pthread_mutex_init(&mtx,NULL);
}
void sockscan_class::startScan(int pthreads) {
    unsigned long int plists[pthreads];
    for (int i = 0; i < pthreads; ++i) {
        pthread_t pid;
        plists[i] = pid;
    }
    for (int j = 0; j < pthreads; ++j) {
        pthread_create(&plists[j],NULL,scan,(void *)this);
        pthread_join(plists[j],NULL);
    }
}
sockscan_class::~sockscan_class() {
    pthread_mutex_destroy(&mtx);
}