#include <fstream>
#include "../include/cpu.hpp"

int
print(int sp, int *_memory) {
    cout << _memory[sp];
    return 0;
}

int
nl(int sp, int *_memory) {
    cout << endl;
    return 0;
}

int
put(int sp, int *_memory) {
    cout << (char)_memory[sp];
    return 0;
}

int
main(int argc, char **argv) {
    int index = 1;
    vector<Code> codes;
    ifstream ifs(argv[1], ios::binary);
    char str[256];
    while(!ifs.eof()) {
        Code code;
        ifs.read((char*)&code, sizeof(Code));
        codes.push_back(code);
    }
    ifs.close();

    CPU cpu;
    cpu.add(print);
    cpu.add(put);
    cpu.add(nl);
    cpu.set(codes);
    return cpu.exe();
}
