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
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int) = 0;

    static void addTab(ostream&, int tab);
};

class DeclVar: public Node {
protected:
    string _id;
public:
    DeclVar(string id): _id(id){}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class DeclArrayVar: public DeclVar {
    int _num;
public:
    DeclArrayVar(string id, int num): DeclVar(id), _num(num) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class Function: public Node {
protected:
    string _id;
    vector<DeclVar *> _args;
    Statement *_body;
public:
    Function(string id, vector<DeclVar *> args, Statement *body)
        : _id(id), _args(args), _body(body) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class ImportFunction: public Function {
public:
    ImportFunction(string id, vector<DeclVar *> args)
        : Function(id, args, NULL) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class Statement: public Node {
public:
    virtual void print(ostream&, int tab) {}
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int) {}
};

class DeclVarSt: public Statement {
    DeclVar *_decl;
public:
    DeclVarSt(DeclVar *decl): _decl(decl){}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class AssignSt: public Statement {
    Expression *_leftside;
    Expression *_expr;
public:
    AssignSt(Expression *leftside, Expression *expr): _leftside(leftside), _expr(expr) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class IfSt: public Statement {
    Expression *_cond;
    Statement *_truest;
    Statement *_falsest;
public:
    IfSt(Expression *cond, Statement *truest, Statement *falsest)
        : _cond(cond), _truest(truest), _falsest(falsest) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class WhileSt: public Statement {
    Expression *_cond;
    Statement *_body;
public:
    WhileSt(Expression *cond, Statement *body)
        : _cond(cond), _body(body) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class ForSt: public Statement {
    Statement    *_init;
    Expression  *_cond;
    Statement    *_proceed;
    Statement   *_body;
public:
    ForSt(Statement *init, Expression *cond, Statement *proceed, Statement *body)
        : _init(init), _cond(cond), _proceed(proceed), _body(body) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class CallFuncSt: public Statement {
    string _id;
    vector<Expression *> _args;
public:
    CallFuncSt(string id, vector<Expression *> args)
        : _id(id), _args(args) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class ReturnSt: public Statement {
    Expression *_exp;
public:
    ReturnSt(Expression *exp)
        : _exp(exp){}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class Block: public Statement {
    vector<Statement*> _statements;
public:
    Block(vector<Statement *> statements): _statements(statements) {}
    
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class Expression: public Node {
public:
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int) {}
};

class AddExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    AddExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class SubExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    SubExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class MulExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    MulExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class DivExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    DivExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class ModExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    ModExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class EQExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    EQExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class NEExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    NEExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class LTExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    LTExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class LEExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    LEExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class GTExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    GTExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class GEExp: public Expression {
    Expression *_left;
    Expression *_right;
public:
    GEExp(Expression *left, Expression *right) :_left(left), _right(right) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class IntExp: public Expression {
    int _int_val;
public:
    IntExp(int int_val) :_int_val(int_val) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class ArrayIndex: public Expression {
    Expression *_pointer;
    Expression *_index;
public:
    ArrayIndex(Expression *pointer, Expression *index) :_pointer(pointer), _index(index) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class Address: public Expression {
    string _id;
public:
    Address(string id): _id(id) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class RightSide: public Expression {
public:
    virtual void print(ostream&, int tab) {}
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int) {}
};

class Access: public RightSide {
    Expression *_rightside;
public:
    Access(Expression *rightside):_rightside(rightside) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class Variable: public RightSide {
    string _id;
public:
    Variable(string id):_id(id) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class VarExp: public Expression {
    Expression *_rightside;
public:
    VarExp(Expression *rightside): _rightside(rightside) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class CallFuncExp: public Expression {
    string _id;
    vector<Expression *> _args;
public:
    CallFuncExp(string id, vector<Expression *> args)
        : _id(id), _args(args) {}
    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class LeftSide: public Expression {
public:
    virtual void print(ostream&, int tab) {}
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int) {}
};

class PointerLeftSide: public LeftSide {
    Expression *_leftside;
public:
    PointerLeftSide(Expression *leftside): _leftside(leftside) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};

class VarLeftSide: public LeftSide {
    string _id;
public:
    VarLeftSide(string id): _id(id) {}

    virtual void print(ostream&, int tab);
    virtual void compile(vector<Code>&, map<string, int>&, map<string, int>&, int);
};
