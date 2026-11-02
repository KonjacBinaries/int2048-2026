#include "../../src/include/int2048.h"
#include "../../src/int2048.cpp"

sjtu::int2048 a, b, c, d;
#include <cassert>
int main() {
     {
        std::cin >> a;
        sjtu::compute_reciprocal(a, b);
        std::cout << b << std::endl;
        std::cout << a * b << std::endl;
    }
    return 0;
}