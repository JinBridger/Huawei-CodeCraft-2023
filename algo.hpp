#pragma once

#include "io.hpp"
#include "robot.hpp"
#include "workbench.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace msc {
    class scheduler {
    public:
        scheduler(std::vector<robot> r) : _robots(r) {}

    private:
        std::vector<robot> _robots;
    };
}  // namespace msc