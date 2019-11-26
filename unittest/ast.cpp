#include "../include/ast.hpp"
#include <vector>

Function *
testFunction() {
    vector<DeclVar *> args;
    args.push_back(new DeclVar("a"));
    vector<Statement*> statements;
    statements.push_back(new ReturnSt(new VarExp(new Variable("a"))));
    Block *block = new Block(statements);

    return new Function("test", args, block);
}

Function *
mainFunction() {
    vector<DeclVar *> args;
    vector<Expression *> call_args;
    call_args.push_back(new IntExp(3));
    vector<Statement*> statements;
    statements.push_back(new DeclVarSt(new DeclVar("a")));
    statements.push_back(new AssignSt(new PointerLeftSide(new VarLeftSide("a")), new AddExp(new VarExp(new Variable("a")), new IntExp(1))));
    statements.push_back(new ReturnSt(new CallFuncExp("test", call_args)));
    Block *block = new Block(statements);

    return new Function("main", args, block);
}

int
main() {
    vector<Expression *> args;
    map<string, int> vars;
    map<string, int> functions;
    std::vector<Function*> program;
    std::vector<Code> codes;
    program.push_back(testFunction());
    program.push_back(mainFunction());
    for(auto function: program) {
        function->compile(codes, vars, functions);
    }
    codes.insert(codes.begin(), Code::makeCode(Code::CALL, functions["main"], 0));
    codes.insert(codes.begin() + 1, Code::makeCode(Code::PUSHR, 2, 0));
    codes.insert(codes.begin() + 2, Code::makeCode(Code::POP, 2, 0));
    codes.insert(codes.begin() + 3, Code::makeCode(Code::EXIT, 0, 0));
    for(auto function: program) {
        function->print(cout, 0);
    }
    ofstream ofs("./bin/ast001.bin");
    for(auto code: codes) {
        code.print(ofs);
        ofs << endl;
    }
    ofs.close();
    return 0;
}
