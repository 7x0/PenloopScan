//
// Created by payloads on 11/26/17.
//
#include "../HeaderFile/controller_class.h"
bool controller::start(int argc, char * argv[]) {
    struct timeval times;
    std::stringstream timeStr;
    timeStr << times.tv_sec;
    gettimeofday(&times,NULL);
    //std::cout << timeStr.str() << std::endl;
    /**
     * 设置参数
     */
    std::string opt("v::d:c:i::t:o:r:h::s:u::"); int option;
    // Debug 调试默认为 false
    debug=false;
    // 默认配置文件目录
    globalOptions->config_file = "/etc/PenloopScan/my.ini";
    // 默认DNS服务器地址
    globalOptions->DnsServer = "202.101.172.35";
    // 默认线程数
    globalOptions->threadNum = 5;
    // 默认输出目录
    globalOptions->output = "./"+timeStr.str();
    // 设置时间戳
    globalOptions->start = times.tv_sec;

    globalOptions->rebuild = false;

    while((option=getopt(argc,argv,opt.c_str())) != -1){
        switch (option){
            case 'v':
                std::cout << argv[0] << "  [Version " << VERSION  << "] by payloads "<< std::endl;
                std::cout <<"Email  : " << "\tpayloads@aliyun.com "<< std::endl;
                std::cout <<"Author : " << "\t倾旋 "<< std::endl;
                exit(EXIT_FAILURE);
            case 'd':
                if(optarg==NULL){
                    help(argv);
                    exit(EXIT_SUCCESS);
                }
                globalOptions->domain = optarg;
                break;
            case 'i':
                globalOptions->rebuild = true;
                break;
            case 'u':
                debug = true;
                break;
            case 't':
                if(optarg==NULL){
                    help(argv);
                    exit(EXIT_FAILURE);
                }
                globalOptions->threadNum = atoi(optarg);
                break;
            case 'o':
                if(optarg==NULL){
                    help(argv);
                    exit(EXIT_FAILURE);
                }
                globalOptions->output = optarg;
                break;
            case 's':
                if(optarg==NULL){
                    help(argv);
                    exit(EXIT_FAILURE);
                }
                globalOptions->DnsServer = optarg;
                break;
            case 'r':
                if(optarg==NULL){
                    Error=NOT_GIVE_DOMAIN_DICT;
                    errorHandle();
                    exit(EXIT_FAILURE);
                }
                globalOptions->dict_path = optarg;
                break;
            case 'h':
                help(argv);
                exit(EXIT_SUCCESS);
            case 'c':
                if(optarg==NULL){
                    help(argv);
                    exit(EXIT_FAILURE);
                }
                globalOptions->config_file = optarg;
                break;
            default:
                help(argv);
                exit(EXIT_SUCCESS);
        }
    }
}

void controller::help(char * argv[]) {
    std::cout << "例子：" << argv[0] << " -dpayloads.online -t5 -rsubnames.txt" << std::endl;
    std::cout << "-d[你要枚举的域名]" << std::endl;
    std::cout << "-s[制定DNS服务器地址](默认是202.101.172.35)" << std::endl;
    std::cout << "-c[配置文件路径](默认是当前目录中的my.ini)" << std::endl;
    std::cout << "-r[字典路径](默认在my.ini中)" << std::endl;
    std::cout << "-u 开启调试模式(无需参数)" << std::endl;
    std::cout << "-t[线程数]" << std::endl;
    std::cout << "-v 输出版本信息" << std::endl;
    std::cout << "-h 输出帮助信息" << std::endl;
    std::cout << "-i 重建数据库，初始化" << std::endl;
    std::cout << "创建日期:\t" << CREATE_TIME << std::endl;
}

void controller::errorHandle() {
    switch (Error){
        case CONFIG_CANT_READ:
            std::cout<< "Config file can't read." << std::endl;
            exit(EXIT_FAILURE);
        case MYSQL_CANT_CONNECT:
            std::cout<< "MySQL can't connect." << std::endl;
            exit(EXIT_FAILURE);
        case MYSQL_SQL_ERROR:
            std::cout<<  "Exec SQL error."<< std::endl;
            exit(EXIT_FAILURE);
        case OPTION_LOST:
            std::cout<< "Some config options lost."<< std::endl;
            exit(EXIT_FAILURE);
        case CONFIG_MYSQL_HOST_LOST:
            std::cout<< "Can't read mysql host from config file."<< std::endl;
            exit(EXIT_FAILURE);
        case CONFIG_MYSQL_USER_LOST:
            std::cout<< "Can't read mysql user from config file."<< std::endl;
            exit(EXIT_FAILURE);
        case CONFIG_MYSQL_PASS_LOST:
            std::cout<< "Can't read mysql password from config file."<< std::endl;
            exit(EXIT_FAILURE);
        case CONFIG_MYSQL_TABLE_LOST:
            std::cout<< "Can't read mysql table name from config file."<< std::endl;
            exit(EXIT_FAILURE);
        case MYSQL_TABLE_NOT_CREATED:
            std::cout<< "MySQL not found table name from config file."<< std::endl;
            exit(EXIT_FAILURE);
        case NOT_GIVE_DOMAIN:
            std::cout<< "Not found domain."<< std::endl;
            exit(EXIT_FAILURE);
        case NOT_GIVE_DOMAIN_DICT:
            std::cout<< "Not found domain dict lists ."<< std::endl;
            exit(EXIT_FAILURE);
        case CONFIG_MYSQL_DATABSE_LOST:
            std::cout<< "Can't read mysql database name from config file."<< std::endl;
            exit(EXIT_FAILURE);
        case CANT_READ_DICT:
            std::cout<< "DICT file can't read."<< std::endl;
            exit(EXIT_FAILURE);
        case PORTS_LOST:
            std::cout<< "Can't read port from config file."<< std::endl;
            exit(EXIT_FAILURE);
        default:
            std::cout<< "Unkown Error. Please contact my  [Email:payloads@aliyun.com]"<< std::endl;
            exit(EXIT_FAILURE);
    }
}

bool controller::readConfigFile(const std::string &key, std::string &value) {
    std::fstream cfgFile;
    cfgFile.open(globalOptions->config_file);
    if( ! cfgFile.is_open())
    {
        return false;
    }
    std::string tmp;
    while(!cfgFile.eof())
    {
        getline(cfgFile,tmp);
        std::string line(tmp);
        size_t pos = line.find('=');
        if(pos==std::string::npos) return false;
        std::string tmpKey = line.substr(0,pos);
        if(key==tmpKey)
        {
            if(line.substr(pos+1) == "null"){
                return true;
            }
            value = line.substr(pos+1);//取=号之后
            cfgFile.close();
            return true;
        }
    }
    cfgFile.close();
    return false;
}

bool controller::readDict() {
    std::fstream IO(globalOptions->dict_path);
    if(!IO.is_open()){
        Error=CANT_READ_DICT;
        errorHandle();
    }
    while(!IO.eof()){
        std::string line;
        getline(IO,line);
        line += "." + globalOptions->domain;
        targetList.push_back(line);
        if(debug){
            std::cout << "[*] " << line << std::endl;
        }
    }
    IO.close();
    if(debug){
        std::cout << "加载字典成功！" << std::endl;
    }
}
void controller::loadConfig() {
    std::fstream IO(globalOptions->config_file);
    if (!IO.is_open()) {
        Error = CONFIG_CANT_READ;
        errorHandle();
    }
    IO.close();
    if (!readConfigFile("mysql_host", globalSet->mysql_host)) {
        Error = CONFIG_MYSQL_HOST_LOST;
        errorHandle();
    }
    if (!readConfigFile("mysql_user", globalSet->mysql_user)) {
        Error = CONFIG_MYSQL_HOST_LOST;
        errorHandle();
    }
    if (!readConfigFile("mysql_pass", globalSet->mysql_pass)) {
        Error = CONFIG_MYSQL_PASS_LOST;
        errorHandle();
    }
    if (!readConfigFile("mysql_database", globalSet->mysql_database)) {
        Error = CONFIG_MYSQL_DATABSE_LOST;
        errorHandle();
    }
    if (!readConfigFile("mysql_table", globalSet->mysql_table)) {
        Error = CONFIG_MYSQL_TABLE_LOST;
        errorHandle();
    }
    if (!readConfigFile("mysql_port", globalSet->mysql_port)) {
        Error = CONFIG_MYSQL_TABLE_LOST;
        errorHandle();
    }
}

controller::controller(){
    globalSet = new GlobalSet;
    globalOptions = new Options;
    Mysql = new mysql_class;
    Dns = new dns_class;
    Sock = new sockscan_class;
    Http = new http_class;
};

controller::~controller(){
    delete Dns;
    delete Sock;
    delete Http;
    delete Mysql;
}

bool controller::ConnectMysql(){
    Mysql->connect(globalSet->mysql_host,globalSet->mysql_user,globalSet->mysql_pass,globalSet->mysql_database,0);
    return true;
}

bool controller::rebulid() {
    Mysql->Update("DROP DATABASE IF EXISTS `PenloopScan`");
    Mysql->Update("CREATE DATABASE IF NOT exists `PenloopScan`");
    Mysql->selectDB("PenloopScan");
    Mysql->Update("DROP TABLE  IF EXISTS `domain`");
    Mysql->Update("CREATE TABLE `domain`(`id` int(10) unsigned NOT NULL AUTO_INCREMENT,`md51` char(32) NOT NULL,`domain` varchar(255) NOT NULL,`ip` varchar(255) NOT NULL,`md52` char(32) NOT NULL,`create_time` timestamp NOT NULL,`ports` varchar(255) DEFAULT NULL,PRIMARY KEY (`id`) USING BTREE,UNIQUE KEY `md51` (`md51`)) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8");
    std::cout << "TableName :[domain]" << std::endl;
    std::cout << "Database  :[PenloopScan]" << std::endl;
    return true;
}