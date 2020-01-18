#ifndef __BLOCKER_HPP__
#define __BLOCKER_HPP__
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

typedef uint32_t ipv4_t;
typedef uint16_t port_t;
typedef uint8_t  byte_t;

typedef struct net {
    port_t min;
    port_t max;
    char tag;
    net(port_t min, port_t max, char tag): min(min), max(max), tag(tag){}
} net;

typedef struct node {
    byte_t cidr;
    ipv4_t mask;
    /* fast lookup for net ids under this cidr */
    map<ipv4_t, net*> table;
    node(byte_t c, ipv4_t m) : cidr(c), mask(m){}
} node;

class Blocker {
    public:
        Blocker(void);
        Blocker(string file);
       ~Blocker(void);
        /* Reads from stream */
        void Build(int fd);
        void AddNode(byte_t cidr, ipv4_t ip, port_t min, port_t max, char tag);
        node* GetNode(byte_t cidr);
        void RemoveNode(byte_t cidr);
        void RemoveNet(ipv4_t ip);
        node* FindNode(ipv4_t ip);
        bool Forward(ipv4_t ip, port_t port);
        void PrintTable(void);
    private:
        /* sorted ascending by cidr notation */
        vector<node*> l;
};
#endif