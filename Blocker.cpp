#include "Blocker.hpp"
#include <algorithm>
#include <regex>
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
        ipv4_t ip;
        byte_t cidr;
        port_t min, max;
        char tag;
        /* g1: ip g2: cidr g3: min_port g4: max_port g5: char */
        regex e ("(.+)\\/(\\d+)\\s+(\\d+)-(\\d+)\\s+(\\w)");
        smatch m;
        while(getline(f, buff)) {
            regex_search(buff,m,e);
            if(m.size() == 6) {
                ip = inet_network(m[1].str().c_str());
                if(ip != -1) {
                    cidr = atoi(m[2].str().c_str());
                    min = atoi(m[3].str().c_str());
                    max = atoi(m[4].str().c_str());
                    tag = m[5].str().c_str()[0];
                   // printf("ip: %u / %u %u - %u %c\n", ip, cidr, min, max, tag);
                    this->AddNode(cidr, ip, min, max, tag);
                } else {
                    cerr << "Invalid IP address\n";
                }
            } else {
                cerr << "format error: " << buff << endl;
            }
            buff.clear();
        }

        /* sort in ascending for quick mask comparison.
         * if higher mask matches then we dont need to 
         * check for lower masks.
         */
         sort(this->l.begin(), this->l.end(), comp);

        f.close();
    } catch(const ifstream::failure& e) {
        cout << "Exception: " << e.what() << endl;
    }

}


/* if node not exists:
 *  add new node
 * else: add net
 */
void Blocker::AddNode(byte_t cidr, ipv4_t ip, port_t min, port_t max, char tag)
{
    node* n = this->GetNode(cidr);
    if(!n) {
        port_t mask = cidr_to_mask(cidr);
        if(mask == -1) {
            cerr << "cidr_to_mask: Invalid bit count\n";
            return;
        }
        n = new node(cidr, mask);
        net* t = new net(ip & mask, min, max, tag);
        n->table[ip & mask] = t;
        this->l.push_back(n);
    } else {
        if(n->table[ip & n->mask]) { 
            n->table[ip & n->mask]->min = min;
            n->table[ip & n->mask]->max = max;
            n->table[ip & n->mask]->tag = tag;
        } else {
            n->table[ip & n->mask] = new net(ip & n->mask, min, max, tag);
        }
    }
}

node* Blocker::GetNode(byte_t cidr)
{
    for(node* n : this->l) {
        if(n->cidr == cidr)
            return n;
    }    

    return NULL;
}

void Blocker::RemoveNode(byte_t cidr)
{
    node* n = this->GetNode(cidr);
    if(!n)
        return;
    for(map<ipv4_t, net*>::iterator it = n->table.begin(); it != n->table.end(); it++) {
        free(it->second);
    }
    n->table.clear();
}

void Blocker::RemoveNet(ipv4_t ip)
{
    node* n = this->FindNode(ip);
    if(!n)
        return;
    n->table.erase(ip & n->mask);
}

node* Blocker::FindNode(ipv4_t ip)
{
    ipv4_t net;
    map<ipv4_t, struct net*>::iterator it;
    for(node* n : this->l) {
        net = n->mask & ip;
        it = n->table.find(net);
        if(it != n->table.end())
            return n;
    }    
    return NULL;
}

void Blocker::PrintTable()
{
    for(node* n : l) {
        for(auto it: n->table) {
            printf("%s/%u %u-%u %c\n", inet_ntoa(it.second->ip), it.second->min, it.second->max, it.second->tag);
        }
    }
}

Blocker::~Blocker()
{
    cout << "Good Bye!\n";
}

bool Blocker::Forward(ipv4_t ip, port_t port)
{
   node* n = this->FindNode(ip);
   net* t = n->table[ip & n->mask];
   return (port >= t->min && port <= t->max && (t->tag == 'f'));
}