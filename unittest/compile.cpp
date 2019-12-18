#include "../include/lexer.hpp"
#include "../include/parser.hpp"

int
main(int argc, char **argv) {
    if(argc > 0) {
        string str;
        ifstream ifs(argv[1]);
        while(!ifs.fail()) {
            string line;
            getline(ifs, line);
            str += line;
        }
        Lexer lexer;
        Parser parser;
        auto tokens = lexer.lex(str.c_str());
        auto program = parser.parse(tokens);

        map<string, int> functions;
        std::vector<Code> codes;
        for(auto function: program) {
            Env env;
            function->compile(codes, env, functions, 4);
        }
        codes.insert(codes.begin(), Code::makeCode(Code::CALL, functions["main"], 0));
        codes.insert(codes.begin() + 1, Code::makeCode(Code::PUSHR, 2, 0));
        codes.insert(codes.begin() + 2, Code::makeCode(Code::POP, 2, 0));
        codes.insert(codes.begin() + 3, Code::makeCode(Code::EXIT, 0, 0));
        for(auto function: program) {
            function->print(cout, 0);
        }
        ofstream ofs(format("./%s.bin", argv[2], ios::binary).c_str());
        for(auto code: codes) { ofs.write((char*)&code, sizeof(Code)); }
        ofs.close();
    }
    return 0;
}
