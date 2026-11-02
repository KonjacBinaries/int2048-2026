#include "../../src/include/int2048.h"
#include "../../src/int2048.cpp"

sjtu::int2048 a, b, c, d;
#include <cassert>
int main() {
    while (1) {
        std::cin >> a >> b;
        std::cout << a / b << " " << a % b << std::endl;
    }
    return 0;
}