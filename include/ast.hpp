#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "code.hpp"
#include "utils.hpp"
using namespace std;

class Expression;
class Statement;

class CompileError: public std::runtime_error {
public:
    CompileError(const char *_Message)
        : runtime_error(_Message) {}
};

class Node {
public:
    virtual void print(ostream&, int tab) = 0;
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&) = 0;

    static void addTab(ostream&, int tab);
};

class DeclVar: public Node {
    string _id;
public:
    DeclVar(string id): _id(id){}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class Function: public Node {
    string _id;
    vector<DeclVar *> _args;
    Statement *_body;
public:
    Function(string id, vector<DeclVar *> args, Statement *body)
        : _id(id), _args(args), _body(body) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class Statement: public Node {
public:
    virtual void print(ostream&, int tab) {}
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&) {}
};

class DeclVarSt: public Statement {
    DeclVar *_decl;
public:
    DeclVarSt(DeclVar *decl): _decl(decl){}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class LeftSide: public Node {
public:
    virtual void print(ostream&, int tab) {}
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&) {}
};

class PointerLeftSide: public LeftSide {
    LeftSide *_leftside;
public:
    PointerLeftSide(LeftSide *leftside): _leftside(leftside) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class VarLeftSide: public LeftSide {
    string _id;
public:
    VarLeftSide(string id): _id(id) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class AssignSt: public Statement {
    LeftSide *_leftside;
    Expression *_expr;
public:
    AssignSt(LeftSide *leftside, Expression *expr): _leftside(leftside), _expr(expr) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class IfSt: public Statement {
    Expression *_cond;
    Statement *_truest;
    Statement *_falsest;
public:
    IfSt(Expression *cond, Statement *truest, Statement *falsest)
        : _cond(cond), _truest(truest), _falsest(falsest) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class WhileSt: public Statement {
    Expression *_cond;
    Statement *_body;
public:
    WhileSt(Expression *cond, Statement *body)
        : _cond(cond), _body(body) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class CallFuncSt: public Statement {
    string _id;
    vector<Expression *> _args;
public:
    CallFuncSt(string id, vector<Expression *> args)
        : _id(id), _args(args) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class ReturnSt: public Statement {
    Expression *_exp;
public:
    ReturnSt(Expression *exp)
        : _exp(exp){}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class Block: public Statement {
    vector<Statement*> _statements;
public:
    Block(vector<Statement *> statements): _statements(statements) {}
    
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class Expression: public Node {
public:
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&) {}
};

class AddExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    AddExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class SubExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    SubExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class MulExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    MulExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class DivExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    DivExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class ModExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    ModExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class EQExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    EQExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class NEExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    NEExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class LTExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    LTExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class LEExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    LEExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class GTExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    GTExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class GEExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    GEExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class IntExp: public Expression {
    int _int_val;
public:
    IntExp(int int_val) :_int_val(int_val) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class Address: public Expression {
    string _id;
public:
    Address(string id): _id(id) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class VarExp: public Expression {
    string _id;
public:
    VarExp(string id): _id(id) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};

class CallFuncExp: public Expression {
    string _id;
    vector<Expression *> _args;
public:
    CallFuncExp(string id, vector<Expression *> args)
        : _id(id), _args(args) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&);
};
