//
// Created by payloads on 11/27/17.
//

#ifndef NEWSCANDNSTODB_DNS_CLASS_H
#define NEWSCANDNSTODB_DNS_CLASS_H

#include <ldns/ldns.h>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include "global.h"
#include "../HeaderFile/dns_class.h"
extern std::vector<struct globalScanResult> GlobalScanData;
extern struct Options * globalOptions;
extern struct GlobalSet * globalSet;

class dns_class{
public:
    void Enum(std::vector<std::string> targetList);
private:
    ldns_resolver   *res;
    ldns_rdf        *name;
    ldns_rdf        *nameserver;
    ldns_rdf        *rr_rdf;
    ldns_pkt        *p;
    ldns_rr_list    *info;
    ldns_rr         *rr;
    ldns_status     s;
};
#endif
