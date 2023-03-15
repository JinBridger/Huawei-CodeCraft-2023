#pragma once

#include "core.hpp"
#include "robot.hpp"
#include "workbench.hpp"

#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace msc {
    class io {
    public:
        io(std::array<robot, ROBOT_N>& robots) : _robots(robots) {
            // Load map (TODO)
            _workbenches.resize(10);
        }

        void send() {
            // Combine outputs
            std::string instructions = "";
            for (int i = 0; i < ROBOT_N; ++i) {
                instructions += _robots[i].state;
            }
            // Add the start and end of output
            instructions = std::to_string(_frame_id) + "\n" + instructions + "OK";
            std::cout << instructions << std::endl;
            std::cerr << "[LOG] Successfully output frame " << _frame_id << std::endl;
        }

        void receive() {
            std::cin >> _frame_id >> _coin_cnt;

            int workbench_cnt;
            std::cin >> workbench_cnt;

            // Input workbench
            for (int i = 0; i < workbench_cnt; ++i) {
                std::cin >> _workbenches[i].type;
                std::cin >> _workbenches[i].x;
                std::cin >> _workbenches[i].y;
                std::cin >> _workbenches[i].left_time;
                std::cin >> _workbenches[i].material_state;
                std::cin >> _workbenches[i].product_state;
            }

            // Input robots
            for (int i = 0; i < ROBOT_N; ++i) {
                std::cin >> _robots[i].workbench_type;
                std::cin >> _robots[i].object_type;

                std::cin >> _robots[i].time_coef;
                std::cin >> _robots[i].coll_coef;

                std::cin >> _robots[i].angle_v;
                std::cin >> _robots[i].linear_vx;
                std::cin >> _robots[i].linear_vy;

                std::cin >> _robots[i].direction;
                std::cin >> _robots[i].x;
                std::cin >> _robots[i].y;

                _robots[i].state = "";  // Reset the robot state
            }

            // Check if ended correctly
            std::string end_of_input;
            std::cin >> end_of_input;
            if (end_of_input != "OK") {
                std::cerr << "[LOG] ERROR: Input ended unexpectedly: " + end_of_input << std::endl;
            }

            std::cerr << "[LOG] Successfully input frame " << _frame_id << std::endl;
        }

    private:
        int _frame_id = 0;
        int _coin_cnt = 0;

        std::array<robot, ROBOT_N>& _robots;
        std::vector<workbench>      _workbenches;
    };
}  // namespace msc