#ifndef _CODE_H_
#define _CODE_H_

#include <iostream>

struct Code {
    int index;
    enum Mnemonic {
        PUSHI = 0,
        PUSHR,
        POP,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        EQ,
        NE,
        LT,
        LE,
        GT,
        GE,
        AND,
        OR,
        JMP,
        JE,
        JNE,
        MOVE,
        MOVEI,
        STORE,
        LOAD,
        CALL,
        INT,
        RET,
        EXIT,
    } mnemonic;
    int op1;
    int op2;
    void print(std::ostream&);
    static Mnemonic getCodeFromName(std::string);
    static Code makeCode(std::string);
    static Code makeCode(Code::Mnemonic, int, int);
};

#endif