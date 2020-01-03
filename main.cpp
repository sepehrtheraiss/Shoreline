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
    ipv4_t ip;
    //string s("192.168.2.1");

    inet_pton(AF_INET, "192.168.2.0", &ip);
    h.push_back(make_pair(ip, 80));
    cout << "192.168.2.1:80\n";  
    cout << "valid: " << b.valid(h.back().first, h.back().second) << endl;
    return 0;
}
