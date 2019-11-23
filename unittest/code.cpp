#include "../include/code.hpp"

int
main() {
    Code code;
    code.mnemonic = Code::PUSHI;
    code.index = 0;
    code.op1 = 1;
    code.op2 = 0;
    
    code.mnemonic = Code::POP;
    code.index = 1;
    code.op1 = 0;
    code.op2 = 0;
    return 0;
}
