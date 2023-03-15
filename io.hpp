#pragma once

#include "robot.hpp"
#include "workbench.hpp"

#include <string>
#include <vector>

namespace msc {
    class io {
    public:
        io();  // Load map (TODO)

        void send();
        void receive();

    private:
        robot*                 _robots[4];
        std::vector<workbench> _workbenches;
    };
}  // namespace msc