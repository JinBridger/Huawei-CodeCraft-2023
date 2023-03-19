#include "algo.hpp"
#include "io.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

using namespace std;

void algo();

// int FINISHED = 0;

// void simple_goto(msc::robot& bot, double x, double y) {
//     if (bot.angle_v == 0 && bot.linear_v == 0 && !FINISHED) {  // STOP
//         double target_angle = atan((y - bot.y) / (x - bot.x));
//         if (y - bot.y > 0 && x - bot.x < 0) {
//             target_angle += PI;
//         }
//         if (y - bot.y < 0 && x - bot.x < 0) {
//             target_angle -= PI;
//         }
//         double delta_angle = target_angle - bot.direction;
//         if (delta_angle > 0) {
//             bot.rotate(1);
//         }
//         else {
//             bot.rotate(-1);
//         }
//         return;
//     }
//     if (bot.linear_v == 0 && !FINISHED) {  // ROTATING
//         double target_angle = atan((y - bot.y) / (x - bot.x));
//         if (y - bot.y > 0 && x - bot.x < 0) {
//             target_angle += PI;
//         }
//         if (y - bot.y < 0 && x - bot.x < 0) {
//             target_angle -= PI;
//         }
//         double delta_angle = target_angle - bot.direction;
//         if (abs(delta_angle) < 0.05) {
//             bot.rotate(0);
//             FINISHED = 1;
//         }
//         return;
//     }
//     if (bot.angle_v == 0 && FINISHED) {
//         bot.forward(5);
//     }
//     if (sqrt((bot.x - x) * (bot.x - x) + (bot.y - y) * (bot.y - y)) < 0.4) {
//         std::cerr << "SUCCESS!";
//         exit(1);
//     }
//     // RUNNING
// }

int get_rand() {
    return rand() % 20 + 1;
}

// void test_robot() {
//     msc::robot r;
//     r.forward(1);
//     r.rotate(2);
//     r.buy();
//     r.sell();
//     r.destroy();
// }

int main() {
    // freopen("C:/Users/26354/Documents/HuaweiCodeCraft/WindowsRelease/maps/1.txt", "r", stdin);
    array<msc::robot, ROBOT_N> robots;
    msc::io                    io(robots);

    robots[0].id = 0;
    robots[1].id = 1;
    robots[2].id = 2;
    robots[3].id = 3;

    msc::robot_controller** debug_controllers = new msc::robot_controller*[4];

    msc::robot_controller debug_controller_0(io, robots[0]);
    msc::robot_controller debug_controller_1(io, robots[1]);
    msc::robot_controller debug_controller_2(io, robots[2]);
    msc::robot_controller debug_controller_3(io, robots[3]);

    debug_controllers[0] = &debug_controller_0;
    debug_controllers[1] = &debug_controller_1;
    debug_controllers[2] = &debug_controller_2;
    debug_controllers[3] = &debug_controller_3;

    while (1) {
        io.receive();
        for (int i = 0; i < ROBOT_N; ++i) {
            if (debug_controllers[i]->is_idle())
                debug_controllers[i]->start_task(0, get_rand());
            else
                debug_controllers[i]->continue_task();
        }
        io.send();
    }

    return 0;
}
