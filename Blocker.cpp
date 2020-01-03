#include "Blocker.hpp"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

static ipv4_t cidr_to_mask(byte_t cidr)
{
    ipv4_t mask;
    if (cidr > 32) 
        return -1; /* Invalid bit count */

    mask = (0xFFFFFFFFUL << (32 - cidr)) & 0xFFFFFFFFUL;
    return mask;
}

bool comp(node* a, node* b)
{
    return(a->cidr > b->cidr);
}

Blocker::Blocker(void)
{
    cout << "Default Constructor\n\tCall read_cidr\n";
}

Blocker::Blocker(string file)
{
    try {
        ifstream f(file);
        if(!f) {
            cerr << "No such file: " << file << '\n';
            return;
        }
        //f.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
        //if ( (f.rdstate() & ifstream::failbit ) != 0 ) {
        int err;
        int index;
        string buff;
        node* n;
        port_t port;
        while(getline(f, buff)) {
            n = new node();
            /* get ip */
            index = buff.find("/");
            n->ip_str = buff.substr(0, index);
            if((err=inet_pton(AF_INET, n->ip_str.c_str(), &n->net))< 1) {
                if(err < 0)
                    perror("Error inet_pton");
                else
                    cerr << "Not in presentation format\n"; 
            }

            /* get cidr notation */
            buff =  buff.substr(index+1); 
            index = buff.find(" "); 
            n->cidr = atoi(buff.substr(0, index).c_str());
            n->mask = cidr_to_mask(n->cidr);
            n->net &= n->mask;

            /* get ports */
            buff = buff.substr(index+1);
            while((index = buff.find(" ")) != string::npos) {
                port = atoi(buff.substr(0, index).c_str());
                table[port].push_back(n);
                buff = buff.substr(index+1); 
            }
        }
        for(map<port_t, vector<node*>>::iterator it = table.begin(); it != table.end();it++) {
            sort(it->second.begin(), it->second.end(), comp);
        }
        
        f.close();
    } catch(const ifstream::failure& e) {
        cout << "Exception: " << e.what() << endl;
    }

}

void Blocker::printTable()
{
    FOREACH_TABLE
        std::cout << it->first << " => [";
        for(node* n: it->second) {
            printf("%s/%u, ",n->ip_str.c_str(), n->cidr);
        }
        cout << "]\n";
    END_TABLE
}

Blocker::~Blocker()
{
    cout << "Good Bye!\n";
}

bool Blocker::valid(ipv4_t ip, port_t port)
{
    FOREACH_TABLE
        if(it->first == port) {
            printf("ip\t\tmask\t\tnet\n");
            for(node* n: it->second) {
                printf("%u\t%u\t%u\n", ip, n->mask, n->net);
                if(ip & n->mask == n->net) {
                    return true;
                }
            }
        }
    END_TABLE
   return false;
}