#include "../include/parser.hpp"
#include "../include/lexer.hpp"

int
main() {
    Parser parser;
    Lexer lexer;
    auto tokens = lexer.lex("func test(int a) { return a } func main() { return 1 + test(2) }");
    auto program = parser.parse(tokens);
    for(auto func: program) {
        func->print(cerr, 0);
    }
    return 0;
}
