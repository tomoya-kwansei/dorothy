#pragma once

#include <iostream>
#include <vector>
#include "error.hpp"
#include "utils.hpp"
using namespace std;

struct VarInfo {
    string _id;     // 変数の名前
    int _access;    // ベースポインタからの距離
    int _size;      // この変数のサイズ

    void print(ostream&);
};

class Env {
    vector<vector<VarInfo>> _variables;
public:
    void append(string, int);
    void staged();
    void unstaged();
    VarInfo& find(string);
    void clear();
    void print(ostream&);
};
