#include <fstream>
#include "../include/cpu.hpp"

int
print(int sp, int *_memory) {
    cerr << _memory[sp] << endl;
    return 3;
}

int
multi_print(int sp, int *_memory) {
    int times = _memory[sp];
    int number = _memory[sp + 1];
    for(int i = 0; i < times; i++) {
        cerr << number << endl;
    }
    return 3;
}

int
main(int argc, char **argv) {
    int index = 1;
    vector<Code> codes;
    ifstream ifs(argv[1]);
    char str[256];
    while(ifs.getline(str, 256 - 1)) {
        char s[128];
        int op1;
        int op2;
        sscanf(str, "%s %d %d", s, &op1, &op2);
        Code code;
        code.mnemonic = Code::getCodeFromName(s);
        code.index = index;
        code.op1 = op1;
        code.op2 = op2;
        codes.push_back(code);
        index++;
    }
    ifs.close();

    CPU cpu;
    cpu.add(print);
    cpu.add(multi_print);
    cpu.set(codes);
    return cpu.exe();
}
