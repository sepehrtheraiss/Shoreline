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
            
            
        }

        /* sort in ascending for quick mask comparison.
         * if higher mask matches then we dont need to 
         * check for lower masks.
         */
            sort(it->second.begin(), it->second.end(), comp);

        f.close();
    } catch(const ifstream::failure& e) {
        cout << "Exception: " << e.what() << endl;
    }

}


/* if node not exists:
 *  add new node
 * else: add net
 */
void Blocker::AddNode(byte_t cidr, ipv4_t ip, port_t min, port_t max, bool forward)
{
    node* n = this->GetNode(cidr);
    if(!n) {
        port_t mask = cidr_to_mask(cidr);
        if(mask == -1) {
            cerr << "cidr_to_mask: Invalid bit count\n";
            return;
        }
        n = new node(cidr, mask);
        net* t = new net(min, max, forward);
        n->table[ip & mask] = t;
        this->l.push_back(n);
    } else {
        n->table[ip & n->mask] = new net(min, max, forward);
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

void Blocker::printTable()
{
    /*
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
    */
}

Blocker::~Blocker()
{
    cout << "Good Bye!\n";
}

bool Blocker::valid(ipv4_t ip, port_t port)
{
   node* n = this->FindNode(ip);
   net* t = n->table[ip & n->mask];
   return (port >= t->min && port <= t->max && t->forward);
}