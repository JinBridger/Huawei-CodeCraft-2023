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
    // freopen("C:/Users/ASUS/Desktop/fake_input.txt", "r", stdin);
    msc::bench_god     benchgod;
    vector<msc::robot> robots;

    for (int i = 0; i < ROBOT_N; ++i)
        robots.push_back(msc::robot(i));

    msc::io io(robots, benchgod);

    // bool iores = io.receive();

    // std::cerr << "[LOG] IORES: " << iores << std::endl;

    // long long max_time = 0;
    while (1) {
        io.receive();
        // auto start = std::chrono::system_clock::now();
        // for (int i = 0; i < ROBOT_N; ++i) {
        //     for (int j = i + 1; j < ROBOT_N; ++j) {
        //         if(robots[i].is_anti_collision() ||robots[j].is_anti_collision()) {
        //             continue;
        //         }
        //         if (robots[i].collision_detect(robots[j])) {
        //             cerr << "[LOG] Robot " << i << " may collision with" << j << endl;
        //         }
        //     }
        // }

        for (int i = 0; i < ROBOT_N; ++i) {
            // if (robots[i].is_anti_collision()) {
            //     cerr << "[LOG] Robot " << i << " is anti collisioning" << endl;
            //     robots[i].anti_collision();
            // }
            // else
            // {
            if (robots[i].is_waiting() && io.get_frame_id() < 8500) {
                msc::point pos = robots[i].pos();
                robots[i].start_task(benchgod.get_task(pos));
            }
            else {
                if (!robots[i].is_waiting())
                    robots[i].continue_task();
            }
            // }
        }
        // End timer
        // auto end = std::chrono::system_clock::now();
        // Calculate time in ms
        // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // std::cerr << "[LOG] CALC TIME: " << duration.count() << " us" << std::endl;
        // max_time = std::max(max_time, duration.count());
        // std::cerr << "[LOG] MAX CALC TIME: " << max_time << " us" << std::endl;
        io.send();
    }

    return 0;
}
