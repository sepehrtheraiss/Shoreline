#include "Blocker.hpp"
#include <arpa/inet.h>
#include <algorithm>
int main(int argc, char** argv) {
    if(argc != 2) {
        cout << "Command: " << argv[0] << " filename\n";
        return 1;
    }

    Blocker b(argv[1]);
    b.PrintTable();

    cout << "Forward: " << b.Forward(3232235776, 8000) << endl;
    b.RemoveNet(3232235776);
    b.PrintTable();
    printf("remove 24\n");
    b.RemoveNode(24);
    b.PrintTable();
    return 0;
}
