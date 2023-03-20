#pragma once

#include "core.hpp"
#include "data_types.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <vector>

namespace msc {
    class io {
    public:
        io(std::vector<robot>& robots, bench_god& benchgod) : _robots(robots), _bench_god(benchgod) {
            // Load map
            int robot_index = 0;
            for (int x = 0; x < MAP_SIZE; ++x) {
                std::string cur_line;
                std::getline(std::cin, cur_line);
                for (int y = 0; y < cur_line.size(); ++y) {
                    if (std::isdigit(cur_line[y]))
                        _bench_god.add_bench(point{ x / 2.0, 50 - y / 2.0 }, cur_line[y] - '0');
                    else if (cur_line[y] == 'A') {
                        _robots[robot_index++].update_pos({ x / 2.0, 50 - y / 2.0 });
                    }
                }
            }

            // Maybe we will do some pretask here
            // PRETASK();
            // Then output OK!

            // Check if ended correctly
            std::string end_of_input;
            std::cin >> end_of_input;
            if (end_of_input != "OK") {
                std::cerr << "[LOG] ERROR: Map input ended unexpectedly: " + end_of_input << std::endl;
            }
            std::cout << "OK" << std::endl;

            std::cerr << "[LOG] Successfully loaded map, " << _bench_god.size() << " workbenches" << std::endl;
        }

        void send() {
            // Combine outputs
            std::string instructions = "";
            for (int i = 0; i < ROBOT_N; ++i) {
                instructions += _robots[i].get_state();
            }
            // Add the start and end of output
            instructions = std::to_string(_frame_id) + "\n" + instructions + "OK";
            std::cout << instructions << std::endl;
            // std::cerr << "[LOG] Successfully output frame " << _frame_id << std::endl;
        }

        void receive() {
            std::cin >> _frame_id >> _coin_cnt;

            int workbench_cnt;
            std::cin >> workbench_cnt;

            // Input workbench
            _bench_god.update();

            // Input robots
            for (int i = 0; i < ROBOT_N; ++i) {
                _robots[i].update();
            }

            // Check if ended correctly
            std::string end_of_input;
            std::cin >> end_of_input;
            if (end_of_input != "OK") {
                std::cerr << "[LOG] ERROR: Input ended unexpectedly: " + end_of_input << std::endl;
                exit(1);
            }

            // std::cerr << "[LOG] Successfully input frame " << _frame_id << std::endl;
        }

    private:
        int _frame_id = 0;
        int _coin_cnt = 0;

        std::vector<robot>& _robots;
        bench_god&          _bench_god;
    };
}  // namespace msc