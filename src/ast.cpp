#include "../include/ast.hpp"
using namespace std;

void 
Node::addTab(ostream& os, int tab) {
    for(int i = 0; i < tab; i++) {
        os << "  ";
    }
}

void 
Expression::print(ostream& os, int tab) {
    os << "<EXP>";
}

void
DeclVar::print(ostream& os, int tab) {
    os << "int " << _id;
}

void
DeclVar::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    env.append(_id, 1);
    ofs.push_back(Code::makeCode(Code::MOVE, 2, 0));
    ofs.push_back(Code::makeCode(Code::PUSHI, env.find(_id)._access, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::SUB, 0, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 1, 2));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void
DeclVar::lcompile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    env.append(_id, 1);
    ofs.push_back(Code::makeCode(Code::MOVE, 2, 0));
    ofs.push_back(Code::makeCode(Code::PUSHI, env.find(_id)._access, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::SUB, 0, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 1, 2));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void
DeclArrayVar::print(ostream& os, int tab) {
    DeclVar::print(os, tab);
    os << "[" << _num << "]";
}

void
DeclArrayVar::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    DeclVar::compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    ofs.push_back(Code::makeCode(Code::PUSHI, _num, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::SUB, 0, 0));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::STORE, 2, 3));
    ofs.push_back(Code::makeCode(Code::MOVE, 1, 3));
    env.find(_id)._size += _num;
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void
DeclArrayVar::lcompile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    DeclVar::lcompile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    ofs.push_back(Code::makeCode(Code::PUSHI, _num, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::SUB, 0, 0));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::STORE, 2, 3));
    ofs.push_back(Code::makeCode(Code::MOVE, 1, 3));
    env.find(_id)._size += _num;
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void 
DeclVarSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    _decl->print(os, tab);
    os << ";" << endl;
}

void 
DeclVarSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _decl->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
}

void 
IfSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << "if(";
    _cond->print(os, tab);
    os << ") ";
    _truest->print(os, tab);
    if(_falsest) {
        Node::addTab(os, tab);
        os << "else ";
        _falsest->print(os, tab);
    }
    os << endl;
}

void 
IfSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    int jumpToElse;
    int jumpFromTrue;
    _cond->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::JNE, 0, 0));
    jumpToElse = ofs.size() - 1;
    _truest->compile(ofs, env, functions, offset);
    ofs[jumpToElse].op1 = ofs.size() + offset - 1;
    if(_falsest) {
        ofs[jumpToElse].op1 = ofs.size() + offset;
        ofs.push_back(Code::makeCode(Code::JMP, 0, 0));
        jumpFromTrue = ofs.size() - 1;
        _falsest->compile(ofs, env, functions, offset);
        ofs[jumpFromTrue].op1 = ofs.size() - 1 + offset;
    }
}

void 
WhileSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << "while(";
    _cond->print(os, tab);
    os << ") ";
    _body->print(os, tab);
    os << endl;
}

void 
WhileSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    int jumpToBottom = 0;
    int top = ofs.size() - 1 + offset;
    _cond->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::JNE, 0, 0));
    jumpToBottom = ofs.size() - 1;
    _body->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::JMP, top, 0));
    ofs[jumpToBottom].op1 = ofs.size() - 1 + offset;
}

void 
ForSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << "for(";
    _init->print(os, tab);
    os << ";";
    _cond->print(os, tab);
    os << ";";
    _proceed->print(os, tab);
    os << ") ";
    _body->print(os, tab);
    os << endl;
}

void 
ForSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    env.staged();
    int jumpToBottom = 0;
    _init->compile(ofs, env, functions, offset);
    int top = ofs.size() + offset - 1;
    _cond->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::JNE, 0, 0));
    jumpToBottom = ofs.size() - 1;
    _body->compile(ofs, env, functions, offset);
    _proceed->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::JMP, top, 0));
    ofs[jumpToBottom].op1 = ofs.size() - 1 + offset;
    env.unstaged();
}

void 
Block::print(ostream& os, int tab) {
    os << "{" << endl;
    for(Statement *statement: _statements) {
        statement->print(os, tab+1);
    }
    Node::addTab(os, tab);
    os << "}" << endl;
}

void 
Block::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    env.staged();
    for(Statement *statement: _statements) {
        statement->compile(ofs, env, functions, offset);
    }
    env.unstaged();
}

void
CallFuncSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << _id << "(";
    if(_args.size() > 0) {
        _args[0]->print(os, tab);
        for(int i = 1; i < _args.size(); i++) {
            os << ", ";
            _args[i]->print(os, tab);
        }
    }
    os << ")" << ";" << endl;
}

void
CallFuncSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    if(_args.size() > 0) {
        for(auto arg: _args) {
            arg->compile(ofs, env, functions, offset);
        }
    }
    if(functions[_id]) {
        ofs.push_back(Code::makeCode(Code::CALL, functions[_id], 0));
        if(_args.size() > 0) {
            for(auto arg: _args) {
                ofs.push_back(Code::makeCode(Code::POP, 3, 0));
            }
        }
        ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    } else {
        throw CompileError(format("undefined function: %s", _id.c_str()).c_str());
    }
}

void
Function::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << "func " << _id << "(";
    if(_args.size() > 0) {
        _args[0]->print(os, tab);
        for(int i = 1; i < _args.size(); i++) {
            os << ", ";
            _args[i]->print(os, tab);
        }
    }
    os << ")";
    _body->print(os, tab);
}

void
Function::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    functions[_id] = ofs.size() - 1 + offset;
    ofs.push_back(Code::makeCode(Code::PUSHR, 0, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 0, 1));
    env.staged();
    if(_args.size() > 0) {
        for(auto arg: _args) {
            arg->compile(ofs, env, functions, offset);
        }
        for(int i = 0; i < _args.size(); i++) {
            ofs.push_back(Code::makeCode(Code::MOVE, 2, 0));
            ofs.push_back(Code::makeCode(Code::MOVEI, 3, i + 2));
            ofs.push_back(Code::makeCode(Code::ADD, 0, 0));
            ofs.push_back(Code::makeCode(Code::LOAD, 2, 2));
            ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
            ofs.push_back(Code::makeCode(Code::MOVE, 2, 0));
            ofs.push_back(Code::makeCode(Code::MOVEI, 3, _args.size() - i));
            ofs.push_back(Code::makeCode(Code::SUB, 0, 0));
            ofs.push_back(Code::makeCode(Code::POP, 3, 0));
            ofs.push_back(Code::makeCode(Code::STORE, 2, 3));
        }
    }
    _body->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::PUSHI, 0, 0));
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 1, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 0, 3));
    ofs.push_back(Code::makeCode(Code::RET, 0, 0));
    env.unstaged();
}

void
ImportFunction::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << "import " << _id << endl;
}

void
ImportFunction::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    cerr << "**IMPORT** " << _id + ".bin" << endl;
    ifstream ifs(_id + ".bin");
    int of = offset + ofs.size();
    char str[256];
    if(ifs.fail()) {
        throw CompileError(format("can't find 'bin' file: %s", _id.c_str()).c_str());
    }
    functions[_id] = ofs.size() - 1 + offset;
    while(ifs.getline(str, 256 - 1)) {
        char s[128];
        int op1;
        int op2;
        sscanf(str, "%s %d %d", s, &op1, &op2);
        Code code;
        code.mnemonic = Code::getCodeFromName(s);
        code.op1 = op1;
        code.op2 = op2;
        if(code.mnemonic == Code::JMP  || 
           code.mnemonic == Code::JNE  || 
           code.mnemonic == Code::CALL ||
           code.mnemonic == Code::JE) {
               code.op1 += of;
           }
        ofs.push_back(code);
    }
    ifs.close();
}

void
ReturnSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    os << "return ";
    _exp->print(os, tab);
    os << ";" << endl;
}

void
ReturnSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _exp->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 1, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::MOVE, 0, 3));
    ofs.push_back(Code::makeCode(Code::RET, 0, 0));
}

void
ExpressionSt::print(ostream& os, int tab) {
    Node::addTab(os, tab);
    _exp->print(os, tab);
    os << ";" << endl;
}

void
ExpressionSt::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _exp->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
}

void 
Assign::print(ostream& os, int tab) {
    _leftside->print(os, tab);
    os << "=";
    _expr->print(os, tab);
}

void 
Assign::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _leftside->lcompile(ofs, env, functions, offset);
    _expr->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::STORE, 2, 3));
    ofs.push_back(Code::makeCode(Code::PUSHR, 3, 0));
}

void 
Assign::lcompile(vector<Code>&, Env&, map<string, int>&, int) {
    print(cerr, 0);
    throw CompileError("can't compile as left side");
}

void
AddExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "+";
    _right->print(os, tab);
}

void
AddExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("ADD 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
AddExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("ADD 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
SubExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "-";
    _right->print(os, tab);
}

void
SubExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("SUB 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
SubExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("SUB 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
MulExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "*";
    _right->print(os, tab);
}

void
MulExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::MUL, 0, 0));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void 
MulExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("MUL 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
DivExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "/";
    _right->print(os, tab);
}

void
DivExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("DIV 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
DivExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("DIV 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
ModExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "%";
    _right->print(os, tab);
}

void
ModExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("MOD 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
ModExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("MOD 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
EQExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "==";
    _right->print(os, tab);
}

void
EQExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("EQ 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
EQExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("EQ 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
NEExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "!=";
    _right->print(os, tab);
}

void
NEExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("NE 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
NEExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("NE 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
LTExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "<";
    _right->print(os, tab);
}

void
LTExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("LT 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
LTExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("LT 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
LEExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << "<=";
    _right->print(os, tab);
}

void
LEExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("LE 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
LEExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("ADD 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
GTExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << ">";
    _right->print(os, tab);
}

void
GTExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("GT 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
GTExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("GT 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
GEExp::print(ostream& os, int tab) {
    _left->print(os, tab);
    os << ">=";
    _right->print(os, tab);
}

void
GEExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _left->compile(ofs, env, functions, offset);
    _right->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode("POP 3 0"));
    ofs.push_back(Code::makeCode("POP 2 0"));
    ofs.push_back(Code::makeCode("GE 0 0"));
    ofs.push_back(Code::makeCode("PUSHR 2 0"));
}

void 
GEExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _left->lcompile(codes, env, functions, offset);
    _right->lcompile(codes, env, functions, offset);
    codes.push_back(Code::makeCode("POP 3 0"));
    codes.push_back(Code::makeCode("POP 2 0"));
    codes.push_back(Code::makeCode("GE 0 0"));
    codes.push_back(Code::makeCode("PUSHR 2 0"));
}

void
IntExp::print(ostream& os, int tab) {
    os << _int_val;
}

void
IntExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    ofs.push_back(Code::makeCode("PUSHI " + to_string(_int_val) + " 0"));
}

void 
IntExp::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    codes.push_back(Code::makeCode(Code::PUSHI, _int_val, 0));
}

void
ArrayIndex::print(ostream& os, int tab) {
    _pointer->print(os, tab);
    os << "[";
    _index->print(os, tab);
    os << "]";
}

void
ArrayIndex::compile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    _pointer->compile(codes, env, functions, offset);
    _index->compile(codes, env, functions, offset);
    codes.push_back(Code::makeCode(Code::POP, 3, 0));
    codes.push_back(Code::makeCode(Code::POP, 2, 0));
    codes.push_back(Code::makeCode(Code::ADD, 0, 0));
    codes.push_back(Code::makeCode(Code::LOAD, 2, 2));
    codes.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void
ArrayIndex::lcompile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _pointer->compile(ofs, env, functions, offset);
    _index->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 3, 0));
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::ADD, 0, 0));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void 
Address::print(ostream& os, int tab) {
    os << "&";
    _exp->print(os, tab);
}

void 
Address::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _exp->lcompile(ofs, env, functions, offset);
}

void 
Address::lcompile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    print(cerr, 0);
    throw CompileError("can't use as left side");
}

void 
Access::print(ostream& os, int tab) {
    os << "*";
    _rightside->print(os, tab);
}

void 
Access::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _rightside->compile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::LOAD, 2, 2));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void 
Access::lcompile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    _rightside->lcompile(ofs, env, functions, offset);
    ofs.push_back(Code::makeCode(Code::POP, 2, 0));
    ofs.push_back(Code::makeCode(Code::LOAD, 2, 2));
    ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
}

void 
Variable::print(ostream& os, int tab) {
    os << _id;
}

void 
Variable::compile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    try {
        codes.push_back(Code::makeCode(Code::MOVE, 2, 0));
        codes.push_back(Code::makeCode(Code::PUSHI, env.find(_id)._access, 0));
        codes.push_back(Code::makeCode(Code::POP, 3, 0));
        codes.push_back(Code::makeCode(Code::SUB, 0, 0));
        codes.push_back(Code::makeCode(Code::LOAD, 2, 2));
        codes.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    } catch(exception& ex) {
        throw CompileError(format("undefined variable: %s", _id.c_str()).c_str());
    }
}

void 
Variable::lcompile(vector<Code>& codes, Env& env, map<string, int>& functions, int offset) {
    try {
        codes.push_back(Code::makeCode(Code::MOVE, 2, 0));
        codes.push_back(Code::makeCode(Code::PUSHI, env.find(_id)._access, 0));
        codes.push_back(Code::makeCode(Code::POP, 3, 0));
        codes.push_back(Code::makeCode(Code::SUB, 0, 0));
        codes.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    } catch(exception& ex) {
        throw CompileError(format("undefined variable: %s", _id.c_str()).c_str());
    }
}

void
CallFuncExp::print(ostream& os, int tab) {
    os << _id << "(";
    if(_args.size() > 0) {
        _args[0]->print(os, tab);
        for(int i = 1; i < _args.size(); i++) {
            os << ", ";
            _args[i]->print(os, tab);
        }
    }
    os << ")";
}

void
CallFuncExp::compile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    if(_args.size() > 0) {
        for(auto arg: _args) {
            arg->compile(ofs, env, functions, offset);
        }
    }
    if(functions[_id]) {
        ofs.push_back(Code::makeCode(Code::CALL, functions[_id], 0));
        if(_args.size() > 0) {
            for(auto arg: _args) {
                ofs.push_back(Code::makeCode(Code::POP, 3, 0));
            }
        }
        ofs.push_back(Code::makeCode(Code::PUSHR, 2, 0));
    } else {
        throw CompileError(format("undefined function: %s", _id.c_str()).c_str());
    }
}

void
CallFuncExp::lcompile(vector<Code>& ofs, Env& env, map<string, int>& functions, int offset) {
    print(cerr, 0);
    throw CompileError("can't use as left side");
}
