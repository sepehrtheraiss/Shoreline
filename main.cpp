#include "Blocker.hpp"
#include <arpa/inet.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        cout << "Command: " << argv[0] << " filename\n";
        return 1;
    }

    Blocker b(argv[1]);
    b.printTable();
    
    vector<pair<ipv4_t, port_t>> h;
    //ipv4_t ip;
    //struct in_addr ip;

    /*
    string s("192.168.1.1");
    ipv4_t ip;// = (192 << 24UL) | (168 << 16UL) | (1 << 8UL) | 0;
    ip = inet_network(s.c_str());
    cout << s << endl; 
    cout << ip << endl;
    h.push_back(make_pair(ip, 80));
    cout << "valid: " << b.valid(h.back().first, h.back().second) << endl;

    s.pop_back();
    s += "20";
    ip = inet_network(s.c_str());
    cout << s << endl; 
    cout << ip << endl;
    h.push_back(make_pair(ip, 80));
    cout << "valid: " << b.valid(h.back().first, h.back().second) << endl;

    ip = inet_network("10.0.0.1");
    cout << "10.0.0.1" << endl; 
    cout << ip << endl;
    h.push_back(make_pair(ip, 80));
    cout << "valid: " << b.valid(h.back().first, h.back().second) << endl;
    */
    return 0;
}
