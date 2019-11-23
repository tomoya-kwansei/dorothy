#pragma once

#include <vector>
#include "token.hpp"

class Lexer {
    vector<Token> tokens;
public:
    vector<Token> lex(string);
private:
    void tokenize(string, int*);
    bool skip(string, int*);
    bool tokenize_int(string, int*);
    bool tokenize_id(string, int*);
    bool tokenize_keyword(string p, int* ppos, string keyword, Token::Type type);
    bool tokenize_operator(string, int*, char);
};
