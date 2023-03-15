#pragma once

#include "core.hpp"
#include "robot.hpp"
#include "workbench.hpp"

#include <array>
#include <string>
#include <vector>

namespace msc {
    class io {
    public:
        io(std::array<robot, ROBOT_N>& robots) : _robots(robots) {
            // Load map (TODO)
        }

        void send();
        void receive();

    private:
        std::array<robot, ROBOT_N>& _robots;
        std::vector<workbench>      _workbenches;
    };
}  // namespace msc