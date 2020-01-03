#include "Blocker.hpp"

int main(int argc, char** argv) {
    if(argc != 2) {
        cout << "Command: " << argv[0] << " filename\n";
        return 1;
    }

    Blocker b(argv[1]);
    b.printTable();
    return 0;
}
