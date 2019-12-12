#include "../include/env.hpp"

int
main() {
    Env env;
    env.staged();
    env.append("a", 1);
    env.append("b", 1);
    env.print(cout);

    env.staged();
    env.append("c", 1);
    env.print(cout);
    env.unstaged();
    env.print(cout);
    env.unstaged();
    env.print(cout);
    return 0;
}
