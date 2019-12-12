#pragma once
#include <iostream>

class CompileError: public std::runtime_error {
public:
    CompileError(std::string _Message)
        : runtime_error(_Message.c_str()) {}
};
