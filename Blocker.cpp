#include "Blocker.hpp"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

static IPv4_t cidr_to_mask(byte_t cidr)
{
    IPv4_t mask;
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
        int index;
        string buff;
        node* n;
        port_t port;
        while(getline(f, buff)) {
            n = new node();
            /* get ip */
            index = buff.find("/");
            n->IP_str = buff.substr(0, index);

            /* get cidr notation */
            buff =  buff.substr(index+1); 
            index = buff.find(" "); 
            n->cidr = atoi(buff.substr(0, index).c_str());
            n->mask = cidr_to_mask(n->cidr);

            /* get ports */
            buff = buff.substr(index+1);
            while((index = buff.find(" ")) != string::npos) {
                port = atoi(buff.substr(0, index).c_str());
                table[port].push_back(n);
                buff = buff.substr(index+1); 
            }
            //net.push_back(n);
        }
        //sort(net.begin(), net.end(), comp);
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
    /*
    for (std::map<string,vector<string>>::iterator it=table.begin(); it!=table.end(); ++it) {
        std::cout << it->first << " => [";
        for(string& ip: it->second) {
            cout << ip << ", ";
        }
        cout << "]\n";
    }
    */
}

Blocker::~Blocker()
{
    cout << "Good Bye!\n";
}