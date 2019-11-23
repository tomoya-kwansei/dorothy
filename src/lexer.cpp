#include "../include/lexer.hpp"

vector<Token> 
Lexer::lex(string p) {
    int pos = 0;
    tokenize(p, &pos);
    tokens.push_back(Token::make_operator(Token::TK_EOF));
    return tokens;
}

void 
Lexer::tokenize(string p, int* ppos) {
    while(p.size() > *ppos) {
        if(skip(p, ppos))                                       continue;
        if(tokenize_keyword(p, ppos, "if", Token::KW_IF))       continue;
        if(tokenize_keyword(p, ppos, "else", Token::KW_ELSE))   continue;
        if(tokenize_keyword(p, ppos, "int", Token::KW_INT))     continue;
        if(tokenize_keyword(p, ppos, "while", Token::KW_WHILE)) continue;
        if(tokenize_keyword(p, ppos, "return", Token::KW_RETURN)) continue;
        if(tokenize_keyword(p, ppos, "func", Token::KW_FUNC))   continue;
        if(tokenize_keyword(p, ppos, "<=", Token::TK_LE))       continue;
        if(tokenize_keyword(p, ppos, ">=", Token::TK_GE))       continue;
        if(tokenize_keyword(p, ppos, "==", Token::TK_EQ))       continue;
        if(tokenize_keyword(p, ppos, "!=", Token::TK_NE))       continue;
        if(tokenize_operator(p, ppos, '='))                     continue;
        if(tokenize_operator(p, ppos, ','))                     continue;
        if(tokenize_operator(p, ppos, '+'))                     continue;
        if(tokenize_operator(p, ppos, '-'))                     continue;
        if(tokenize_operator(p, ppos, '*'))                     continue;
        if(tokenize_operator(p, ppos, '/'))                     continue;
        if(tokenize_operator(p, ppos, '%'))                     continue;
        if(tokenize_operator(p, ppos, '>'))                     continue;
        if(tokenize_operator(p, ppos, '<'))                     continue;
        if(tokenize_operator(p, ppos, '{'))                     continue;
        if(tokenize_operator(p, ppos, '}'))                     continue;
        if(tokenize_operator(p, ppos, '('))                     continue;
        if(tokenize_operator(p, ppos, ')'))                     continue;
        if(tokenize_id(p, ppos))                               continue;
        if(tokenize_int(p, ppos))                               continue;
        cout << format("%c: %s", p[*ppos], p.c_str()) << endl;
        exit(-1);
    }
}

bool 
Lexer::skip(string p, int* ppos) {
    if(p[*ppos] == ' ' || p[*ppos] == '\n') {
        (*ppos)++;
        return true;
    }
    return false;
}

bool 
Lexer::tokenize_int(string p, int* ppos) {
    int start = *ppos;
    if(p[start] < '0' || p[start] > '9') return false;
    while(p[*ppos] >= '0' && p[*ppos] <= '9') (*ppos)++;
    string str = p.substr(start, *ppos - start);
    tokens.push_back(Token::make_int(stoi(str)));
    return true;
}

bool 
Lexer::tokenize_id(string p, int* ppos) {
    int start = *ppos;
    if(p[start] < 'A' || p[start] > 'z') return false;
    while(p[*ppos] >= 'A' && p[*ppos] <= 'z') (*ppos)++;
    string str = p.substr(start, *ppos - start);
    tokens.push_back(Token::make_id(str));
    return true;
}

bool 
Lexer::tokenize_keyword(string p, int* ppos, string keyword, Token::Type type) {
    if(p.substr(*ppos, keyword.size()) == keyword) {
        (*ppos) += keyword.size();
        tokens.push_back(Token::make_operator(type));
        return true;
    } else {
        return false;
    }
}

bool 
Lexer::tokenize_operator(string p, int* ppos, char c) {
    if(p[*ppos] == c) {
        (*ppos)++;
        tokens.push_back(Token::make_operator((Token::Type)c));
        return true;
    }
    else return false;
}
