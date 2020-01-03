#include "Blocker.hpp"
#include <algorithm>
#include <stdio.h>
#include <string.h>

static ipv4_t cidr_to_mask(byte_t cidr)
{
    ipv4_t mask;
    if (cidr > 32) 
        return -1; /* Invalid bit count */
    mask = (0xFFFFFFFF << (32 - cidr)) & 0xFFFFFFFF;
    return mask;
}

static bool comp(node* a, node* b)
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
        string ipstr;
        ipv4_t net;
        ipv4_t mask;
        byte_t cidr;
        port_t port;
        while(getline(f, buff)) {
            /* get ip */
            index = buff.find("/");
            ipstr = buff.substr(0, index);
            net   = inet_network(ipstr.c_str());

            /* get cidr notation */
            buff =  buff.substr(index+1); 
            index = buff.find(" "); 
            cidr = atoi(buff.substr(0, index).c_str());
            mask = cidr_to_mask(cidr);
            net &= mask;

            /* get ports */
            buff = buff.substr(index+1);
            while((index = buff.find(" ")) != string::npos) {
                port = atoi(buff.substr(0, index).c_str());
                //cout << ipstr << "/" <<(int)cidr<<":" << port << endl;
                n = GetNode(port, cidr);
                if(!n) {
                    n = new node(cidr, mask);
                    n->net[net] = true;
                    table[port].push_back(n);
                } else {
                    n->net[net] = true;
                }
                /* set to next port */
                buff = buff.substr(index+1); 
            }
        }

        /* sort in ascending for quick mask comparison.
         * if higher mask matches then we dont need to 
         * check for lower masks.
         */
        FOREACH_TABLE
            sort(it->second.begin(), it->second.end(), comp);
        END_TABLE 

        f.close();
    } catch(const ifstream::failure& e) {
        cout << "Exception: " << e.what() << endl;
    }

}

node* Blocker::GetNode(port_t port, byte_t cidr)
{
    map<port_t, vector<node*>>::iterator it;
    it = table.find(port);
    if(it == table.end())
        return NULL;
    
    for(node* n : it->second) {
        if(n->cidr == cidr)
            return n;
    }

    return NULL;
}
void Blocker::printTable()
{
    in_addr ip;
    FOREACH_TABLE
        std::cout << "\n" << it->first << " => [\n";
        for(node* n: it->second) {
            ip.s_addr = htonl(n->mask);
            cout << "cidr: " << (int)n->cidr << " mask: " << inet_ntoa(ip) << endl;
            for(auto it : n->net) {
               ip.s_addr = htonl(it.first);
               cout << "\t net: " << inet_ntoa(ip) << endl;
            }
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
    for(node* n : table[port]) {
        if(n->net[ip & n->mask]) {
            return true;
        } else {
            /* std::map adds entry even if it's not assigned */
            n->net.erase(ip & n->mask);
        }
    }
   return false;
}