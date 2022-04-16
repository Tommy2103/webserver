#include <cstdio>
#include <iostream>
#include <thread>

void f() {
    
}

int main(int argc, char** argv) {
    std::thread t(f);
    t.detach();
    getchar();
    return 0;
}