#pragma once

#include "token.hpp"
#include "ast.hpp"
#include "utils.hpp"

class ParseError: public runtime_error {
    Token _token;
public:
    ParseError(const char *_Message, Token token)
        : runtime_error(_Message), _token(token) {}
    ParseError(string _Message, Token token)
        : runtime_error(_Message.c_str()), _token(token) {}
};

class Parser {
    int _pos;
    vector<Function *> _functions;
public:
    vector<Function *> parse(vector<Token>& tokens);
private:
    Token consume(vector<Token>&, Token::Type);

    Function *parse_function(vector<Token>&);
    vector<DeclVar *> parse_declargs(vector<Token>&);
    DeclVar *parse_declvar(vector<Token>&);
    Block *parse_block(vector<Token>&);

    LeftSide  *parse_leftside(vector<Token>&);

    Statement *parse_statement(vector<Token>&);
    Statement *parse_declvarst(vector<Token>&);
    Statement *parse_assignst(vector<Token>&);
    Statement *parse_ifst(vector<Token>&);
    Statement *parse_whilest(vector<Token>&);
    Statement *parse_returnst(vector<Token>&);
    Statement *parse_callst(vector<Token>& tokens);

    Expression *parse_expression(vector<Token>&);
    Expression *parse_eq(vector<Token>&);
    Expression *parse_add(vector<Token>&);
    Expression *parse_mul(vector<Token>&);
    Expression *parse_term(vector<Token>&);
    Expression *parse_integer(vector<Token>&);
    Expression *parse_variable(vector<Token>&);
    Expression *parse_call(vector<Token>&);
    vector<Expression *> parse_arg(vector<Token>&);
};
