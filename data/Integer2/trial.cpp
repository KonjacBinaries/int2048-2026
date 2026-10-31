#include "../../src/include/int2048.h"
#include "../../src/int2048.cpp"

sjtu::int2048 a, b, c;
#include <cassert>
int main() {
    unsigned long long c = 0;
    while(1) {
        std::cin >> a >> b;
        a *= b;
        std::cout << a << std::endl;
    }
    return 0;
}