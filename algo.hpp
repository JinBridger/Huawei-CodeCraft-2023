#pragma once

#include "data_types.hpp"
#include "io.hpp"

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