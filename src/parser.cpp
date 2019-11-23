#include "../include/parser.hpp"

vector<Function *> 
Parser::parse(vector<Token>& tokens) {
    _pos = 0;
    while(consume(tokens, Token::TK_EOF).type == Token::NONE) {
        _functions.push_back(parse_function(tokens));
    }
    return _functions;
}

Function * 
Parser::parse_function(vector<Token>& tokens) {
    if(consume(tokens, Token::KW_FUNC).type == Token::NONE) throw ParseError(format("position: %d", _pos), tokens[_pos]);
    Token id_token = consume(tokens, Token::TK_ID);
    if(id_token.type == Token::NONE) throw ParseError(format("position: %d", _pos), tokens[_pos]);
    auto declargs = parse_declargs(tokens);
    auto block = parse_block(tokens);
    return new Function(id_token.id, declargs, block);
}

vector<DeclVar *> 
Parser::parse_declargs(vector<Token>& tokens) {
    vector<DeclVar *> declargs;
    if(consume(tokens, (Token::Type)'(').type == Token::NONE) throw ParseError(format("position: %d", _pos), tokens[_pos]);
    if(tokens[_pos].type == Token::KW_INT) {
        declargs.push_back(parse_declvar(tokens));
        while(consume(tokens, (Token::Type)',').type == (Token::Type)',') {
            declargs.push_back(parse_declvar(tokens));
        }
    }
    if(consume(tokens, (Token::Type)')').type == Token::NONE) throw ParseError(format("position: %d", _pos), tokens[_pos]);
    return declargs;
}

DeclVar *
Parser::parse_declvar(vector<Token>& tokens) {
    if(consume(tokens, Token::KW_INT).type == Token::NONE) return NULL;
    Token id_token = consume(tokens, Token::TK_ID);
    if(id_token.type == Token::NONE) throw ParseError(format("position: %d", _pos), tokens[_pos]);
    return new DeclVar(id_token.id);
}

Block * 
Parser::parse_block(vector<Token>& tokens) {
    vector<Statement *> statements;
    if(consume(tokens, (Token::Type)'{').type == Token::NONE) return NULL;
    while(tokens[_pos].type != (Token::Type)'}') {
        auto st = parse_statement(tokens);
        if(st)
            statements.push_back(st);
        else {
            throw ParseError(format("position: %d", _pos), tokens[_pos]);
        }

    }
    if(consume(tokens, (Token::Type)'}').type == Token::NONE) throw ParseError(format("position: %d", _pos), tokens[_pos]);
    return new Block(statements);
}

Statement *
Parser::parse_declvarst(vector<Token>& tokens) {
    auto decl = parse_declvar(tokens);
    if(!decl) {
        return NULL;
    } else {
        return new DeclVarSt(decl);
    }
}

Statement *
Parser::parse_assignst(vector<Token>& tokens) {
    Token id_token = tokens[_pos];
    Token eq_token = tokens[_pos + 1];
    if(id_token.type != Token::TK_ID || eq_token.type != (Token::Type)'=') return NULL;
    _pos += 2;
    auto exp = parse_expression(tokens);
    return new AssignSt(id_token.id, exp);
}

Statement *
Parser::parse_statement(vector<Token>& tokens) {
    Statement *statement;
    if((statement = parse_declvarst(tokens))) return statement;
    if((statement = parse_assignst(tokens))) return statement;
    if((statement = parse_block(tokens))) return statement;
    if((statement = parse_ifst(tokens))) return statement;
    if((statement = parse_whilest(tokens))) return statement;
    if((statement = parse_returnst(tokens))) return statement;
    if((statement = parse_callst(tokens))) return statement;
    return NULL;
}

Statement *
Parser::parse_returnst(vector<Token>& tokens) {
    if(consume(tokens, Token::KW_RETURN).type == Token::NONE) return NULL;
    Expression *exp = parse_expression(tokens);
    if(!exp) throw ParseError(format("exprected 'expression' at %d", _pos), tokens[_pos]);
    return new ReturnSt(exp);
}

Statement *
Parser::parse_ifst(vector<Token>& tokens) {
    Statement *true_statement = NULL;
    Statement *else_statement = NULL;
    if(consume(tokens, Token::KW_IF).type == Token::NONE) return NULL;
    if(consume(tokens, (Token::Type)'(').type == Token::NONE) 
        throw ParseError(format("expected '(' at %c", _pos), tokens[_pos]);
    auto exp = parse_expression(tokens);
    if(!exp) throw ParseError(format("exprected 'expression' at %d", _pos), tokens[_pos]);
    if(consume(tokens, (Token::Type)')').type == Token::NONE) 
        throw ParseError(format("expected ')' at %d", _pos), tokens[_pos]);
    true_statement = parse_statement(tokens);
    if(!true_statement) throw ParseError(format("exprected 'statement' at %d", _pos), tokens[_pos]);
    if(consume(tokens, Token::KW_ELSE).type != Token::NONE) {
        else_statement = parse_statement(tokens);
    }
    return new IfSt(exp, true_statement, else_statement);
}

Statement *
Parser::parse_whilest(vector<Token>& tokens) {
    if(consume(tokens, Token::KW_WHILE).type == Token::NONE) return NULL;
    if(consume(tokens, (Token::Type)'(').type == Token::NONE) 
        throw ParseError(format("expected '(' at %c", _pos), tokens[_pos]);
    auto exp = parse_expression(tokens);
    if(!exp) throw ParseError(format("exprected 'expression' at %d", _pos), tokens[_pos]);
    if(consume(tokens, (Token::Type)')').type == Token::NONE) 
        throw ParseError(format("expected ')' at %d", _pos), tokens[_pos]);
    Statement *body = parse_statement(tokens);
    return new WhileSt(exp, body);
}

Statement *
Parser::parse_callst(vector<Token>& tokens) {
    Token id_token = tokens[_pos];
    Token lp = tokens[_pos + 1];
    if(id_token.type != Token::TK_ID || lp.type != (Token::Type)'(') return NULL;
    _pos++;
    vector<Expression *> args = parse_arg(tokens);
    return new CallFuncSt(id_token.id, args);
}

Expression *
Parser::parse_eq(vector<Token>& tokens) {
    Expression *exp = parse_add(tokens);
    if(consume(tokens, Token::TK_EQ).type != Token::NONE) {
        return new EQExp(exp, parse_eq(tokens));
    } else if(consume(tokens, Token::TK_NE).type != Token::NONE) {
        return new NEExp(exp, parse_eq(tokens));
    } else if(consume(tokens, Token::TK_LE).type != Token::NONE) {
        return new LEExp(exp, parse_eq(tokens));
    } else if(consume(tokens, Token::TK_GE).type != Token::NONE) {
        return new GEExp(exp, parse_eq(tokens));
    } else if(consume(tokens, (Token::Type)'<').type != Token::NONE) {
        return new LTExp(exp, parse_eq(tokens));
    } else if(consume(tokens, (Token::Type)'>').type != Token::NONE) {
        return new GTExp(exp, parse_eq(tokens));
    } else {
        return exp;
    }
}

Expression *
Parser::parse_add(vector<Token>& tokens) {
    Expression *exp = parse_mul(tokens);
    if(consume(tokens, (Token::Type)'+').type != Token::NONE) {
        return new AddExp(exp, parse_add(tokens));
    } else if(consume(tokens, (Token::Type)'-').type != Token::NONE) {
        return new SubExp(exp, parse_add(tokens));
    } else {
        return exp;
    }
}
Expression *
Parser::parse_mul(vector<Token>& tokens) {
    Expression *exp = parse_term(tokens);
    if(consume(tokens, (Token::Type)'*').type != Token::NONE) {
        return new MulExp(exp, parse_add(tokens));
    } else if(consume(tokens, (Token::Type)'/').type != Token::NONE) {
        return new DivExp(exp, parse_add(tokens));
    } else if(consume(tokens, (Token::Type)'%').type != Token::NONE) {
        return new ModExp(exp, parse_add(tokens));
    } else {
        return exp;
    }
}

Expression *
Parser::parse_term(vector<Token>& tokens) {
    Expression *exp;
    if((exp = parse_call(tokens))) return exp;
    if((exp = parse_variable(tokens))) return exp;
    if((exp = parse_integer(tokens))) return exp;
    return NULL;
}

Expression *
Parser::parse_expression(vector<Token>& tokens) {
    Expression *exp;
    if((exp = parse_eq(tokens))) return exp;
    return NULL;
}

Expression *
Parser::parse_integer(vector<Token>& tokens) {
    Token token = consume(tokens, Token::TK_INT);
    if(token.type == 0) return NULL;
    return new IntExp(token.int_val);
}

Expression *
Parser::parse_variable(vector<Token>& tokens) {
    Token token = consume(tokens, Token::TK_ID);
    if(token.type == Token::TK_ID) {
        return new VarExp(token.id);
    } else {
        return NULL;
    }
}

vector<Expression *> 
Parser::parse_arg(vector<Token>& tokens) {
    vector<Expression *> exprs;
    if(consume(tokens, (Token::Type)'(').type == Token::NONE) 
        throw ParseError(format("expected '(' at %d", _pos), tokens[_pos]);
    auto exp = parse_expression(tokens);
    if(exp) {
        exprs.push_back(exp);
        while(consume(tokens, (Token::Type)',').type != Token::NONE) {
            exp = parse_expression(tokens);
            if(!exp) throw ParseError(format("expected 'expression' at %d", _pos), tokens[_pos]);
            exprs.push_back(exp);
        }
    }
    if(consume(tokens, (Token::Type)')').type == Token::NONE) 
        throw ParseError(format("expected ')' at %d", _pos), tokens[_pos]);
    return exprs;
}

Expression *
Parser::parse_call(vector<Token>& tokens) {
    Token id_token = tokens[_pos];
    Token lp = tokens[_pos + 1];
    if(id_token.type != Token::TK_ID || lp.type != (Token::Type)'(') return NULL;
    _pos++;
    vector<Expression *> args = parse_arg(tokens);
    return new CallFuncExp(id_token.id, args);
}

Token 
Parser::consume(vector<Token>& tokens, Token::Type type) {
    if(tokens[_pos].type == type) {
        _pos++;
        return tokens[_pos - 1];
    } else {
        return Token::none();
    }
}
