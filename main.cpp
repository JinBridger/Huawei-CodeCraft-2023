// #include "algo.hpp"
#include <stdio.h>
#define DEBUG
#include "algo.hpp"
#include "data_types.hpp"
#include "io.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

// int get_rand() {
//     return rand() % 20;
// }

int main() {
     //freopen("C:/Users/ASUS/Desktop/fake_input.txt", "r", stdin);
    msc::bench_god     benchgod;
    vector<msc::robot> robots;

    for (int i = 0; i < ROBOT_N; ++i)
        robots.push_back(msc::robot(i));

    msc::io io(robots, benchgod);

    // bool iores = io.receive();

    // std::cerr << "[LOG] IORES: " << iores << std::endl;

    while (1) {
        io.receive();
        for (int i = 0; i < ROBOT_N; ++i) {
            if (robots[i].is_waiting()) {
                msc::point pos = robots[i].pos();
                robots[i].start_task(benchgod.get_task(pos));
            }
            else
                robots[i].continue_task();
        }
        io.send();
    }

    return 0;
}
