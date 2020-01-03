#ifndef __BLOCKER_HPP__
#define __BLOCKER_HPP__
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

typedef uint32_t ipv4_t;
typedef uint16_t port_t;
typedef uint8_t  byte_t;

#define FOREACH_TABLE for (std::map<port_t,vector<node*>>::iterator it=table.begin(); it!=table.end(); ++it) {
#define END_TABLE }

typedef struct node {
    ipv4_t net;
    ipv4_t mask;
    byte_t cidr;
    string ip_str;
} node;

class Blocker {
    public:
        Blocker(void);
        Blocker(string file);
       ~Blocker(void);
        bool valid(string ipv4_Port);
        bool valid(string ipv4, string port);
        bool valid(ipv4_t ip, port_t port);
        void printTable(void);
    private:
        map<port_t, vector<node*>> table;
};
#endif