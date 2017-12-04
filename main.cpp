#include <iostream>
#include "HeaderFile/global.h"
#include "HeaderFile/controller_class.h"
std::vector<struct globalScanResult> GlobalScanData;
struct Options * globalOptions;
struct GlobalSet * globalSet;
int main(int argc,char * argv[]) {
    controller C;
    C.debug = false;
    C.Mysql->mysql_debug = true;
    C.start(argc,argv);
    C.loadConfig();
    C.ConnectMysql();
    if(globalOptions->rebuild){
        C.rebulid();
        exit(EXIT_SUCCESS);
    }
    C.readDict();
    C.Dns->Enum(C.targetList);
    C.Sock->startScan(1);
    for (int i = 0; i < GlobalScanData.size() ; ++i) {
        std::cout << GlobalScanData[i].domain << " -> " << GlobalScanData[i].IP << "  Port :";
        for (int j = 0; j < GlobalScanData[i].Port.size() ; ++j) {
            std::cout << GlobalScanData[i].Port[j];
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    C.Sock->IntoDatabase(C.Mysql,globalSet->mysql_table);
    C.Http->loadthread();
    delete globalSet;
    delete globalOptions;
    return 0;
}