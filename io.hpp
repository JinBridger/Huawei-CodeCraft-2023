#pragma once

#include "algo.hpp"
#include "core.hpp"
#include "data_types.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
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
            _bench_god.analyze();
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

            std::cerr << "[LOG] Successfully output frame " << _frame_id << std::endl;
        }

        void receive() {
            // if (_frame_id == 327) {
            // std::vector<std::string> str;
            // std::stringstream        ss;
            // while (true) {

            //     std::string tmp;
            //     std::getline(std::cin, tmp);
            //     if (tmp == "OK") {
            //         str.push_back(tmp);
            //         bool chk = check_valid_input(str);
            //         if (!chk) {
            //             std::cerr << "[LOG] ERROR: Input invalid!" << std::endl;
            //             return false;
            //         }
            //         for (auto& s : str) {
            //             ss << s << "\n";
            //             std::cerr << s << std::endl;
            //         }
            //         break;
            //     }
            //     else {
            //         str.push_back(tmp);
            //     }
            // }
            // }
            std::cin >> _frame_id >> _coin_cnt;
            // std::cerr << "[LOG] FRAMEID&CCNT: " << _frame_id << " " << _coin_cnt << std::endl;

            int workbench_cnt;
            std::cin >> workbench_cnt;

            // Input workbench
            std::cin >> _bench_god;

            // Input robots
            for (int i = 0; i < ROBOT_N; ++i) {
                std::cin >> _robots[i];
            }

            // Check if ended correctly
            std::string end_of_input;
            std::cin >> end_of_input;
            std::cerr << end_of_input << std::endl;
            if (end_of_input != "OK") {
                std::cerr << "[LOG] ERROR: Input ended unexpectedly: " + end_of_input << std::endl;
                exit(1);
            }

            std::cerr << "[LOG] Successfully input frame " << _frame_id << std::endl;
        }

        bool check_valid_input(std::vector<std::string> inp) {
            std::string bench_cnt_str = inp[1];
            int         bench_cnt     = std::stoi(bench_cnt_str);
            std::cerr << "[LOG] BENCNT: " << bench_cnt << std::endl;
            std::cerr << "[LOG] TOTLIN: " << inp.size() << std::endl;
            if (inp.size() != bench_cnt + 7) {
                return false;
            }
            return true;
        }

    private:
        int _frame_id = 0;
        int _coin_cnt = 0;

        std::vector<robot>& _robots;
        bench_god&          _bench_god;
    };
}  // namespace msc