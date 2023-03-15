#pragma once

#include "core.hpp"
#include "robot.hpp"
#include "workbench.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace msc {
    class io {
    public:
        io(std::array<robot, ROBOT_N>& robots) : _robots(robots) {
            // Load map
            int robot_index = 0;
            for (int x = 0; x < MAP_SIZE; ++x) {
                std::string cur_line;
                std::getline(std::cin, cur_line);
                for (int y = 0; y < cur_line.size(); ++y) {
                    if (std::isdigit(cur_line[y]))
                        _workbenches.emplace_back(x / 2.0, y / 2.0, cur_line[y] - '0');
                    else if (cur_line[y] == 'A') {
                        _robots[robot_index].x = x / 2.0;
                        _robots[robot_index].y = y / 2.0;
                    }
                }
            }
        }

        void send() {
            // Combine outputs
            std::ostringstream os;
            os << _frame_id << "\n";
            std::copy(_robots.begin(), _robots.end(), std::ostream_iterator<std::string>(os, "\n"));
            std::cout << os.str() << "\nOK" << std::endl;
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
                std::cin >> _robots[i].workbench;
                std::cin >> _robots[i].item;

                std::cin >> _robots[i].time_coef;
                std::cin >> _robots[i].coll_coef;

                std::cin >> _robots[i].angle_v;
                double vx, vy;
                std::cin >> vx >> vy;
                _robots[i].linear_v = sqrt(vx * vx + vy * vy);

                std::cin >> _robots[i].direction;
                std::cin >> _robots[i].x;
                std::cin >> _robots[i].y;
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