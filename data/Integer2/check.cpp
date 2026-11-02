#include <iostream>
#include <cstdio>
#include <string>
#include <random>
std::mt19937 rnd(time(0));
std::string awa, bwb;
int main() {
    int T = 300;
    system("g++ trial.cpp -O2 -o a");
    while (T--) {
        system("touch data.txt");
        int length_a = 1100;
        awa.clear();bwb.clear();
        awa.push_back(rnd() % 4 + 5 + '0');
        for (int i = 1; i < length_a; ++i) {
            awa.push_back(rnd() % 10 + '0');
        }
        // bwb.push_back(rnd() % 9 + 1 + '0');
        // for (int i = 1; i < length_b; ++i) {
        //     bwb.push_back(rnd() % 10 + '0');
        // }
        std::string command = "echo \"" + awa + "\" > data.txt";
        system(command.c_str());
        // system("python check.py < data.txt > python.txt");
        // std::cout << "py done.\n";
        system("./a < data.txt > my.txt");
        // std::cout << "my done.\n";
        // system("diff python.txt my.txt");
        // std::cout << "good.\n";
    }
    return 0;
}