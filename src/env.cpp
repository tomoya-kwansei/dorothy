#include "../include/env.hpp"

void 
Env::append(string id, int size) {
    VarInfo _info;
    int sum = 0;
    for(auto stage: _variables) {
        for(auto info: stage) {
            sum += info._size;
            if(info._id == id) {
                throw CompileError(format("redefinition variable: %s", id.c_str()));
            }
        }
    }
    _info._id = id;
    _info._access = sum + 1;
    _info._size = size;
    _variables[_variables.size() - 1].push_back(_info);
}

void 
Env::staged() {
    vector<VarInfo> stage;
    _variables.push_back(stage);
}

void 
Env::unstaged() {
    vector<VarInfo> stage;
    _variables.pop_back();
}

VarInfo& 
Env::find(string id) {
    for(int i = _variables.size() - 1; i >= 0; i--) {
        for(int j = 0; j < _variables[i].size(); j++) {
            if(_variables[i][j]._id == id) {
                return _variables[i][j];
            }
        }
    }
    throw CompileError(format("undefind variable: %s", id.c_str()));
}

void 
Env::clear() {
    _variables.clear();
}

void 
Env::print(ostream& os) {
    os << "================" << endl;
    for(int i = _variables.size() - 1; i >= 0; i--) {
        os << "----------------" << endl;
        for(auto info: _variables[i]) {
            info.print(os);
        }
    }
}

void 
VarInfo::print(ostream& os) {
    os << "[";
    os << _id;
    os << " ";
    os << _access;
    os << " ";
    os << _size;
    os << "]";
    os << endl;
}
