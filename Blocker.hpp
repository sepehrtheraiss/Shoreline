#ifndef __BLOCKER_HPP__
#define __BLOCKER_HPP__
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

typedef uint32_t IPv4_t;
typedef uint16_t port_t;
typedef uint8_t  byte_t;

typedef struct node {
    IPv4_t net;
    IPv4_t mask;
    byte_t cidr;
    vector<port_t> port;
    string IP_str;
} node;

class Blocker {
    public:
        Blocker(void);
        Blocker(string file);
       ~Blocker(void);
        bool valid(string IPv4_Port);
        bool valid(string IPv4, string port);
        bool valid(IPv4_t IP, port_t port);
        void printTable(void);
    private:
        vector<node*> blist;
};
#endif