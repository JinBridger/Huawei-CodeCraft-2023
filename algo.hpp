#pragma once

#include "data_types.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iostream>
#include <optional>
#include <vector>

namespace msc {
    class bench_god {
    public:
        bench_god() {
            _workbenches.reserve(50);
        }

        void add_bench(const point& pos, int type) {
            _workbenches.emplace_back(pos, type);
        }

        void analyze() {
            for (int i = 0; i < _workbenches.size(); ++i) {
                if (can_product(_workbenches[i]._bench.type())) {
                    for (int j = 0; j < _workbenches.size(); ++j) {
                        if (i != j && can_purchase(_workbenches[j]._bench.type(), _workbenches[i]._bench.type()))
                            _workbenches[i]._tasks.emplace_back(std::addressof(_workbenches[j]._bench));
                    }
                }
            }
        }

        size_t size() noexcept {
            return _workbenches.size();
        }

        friend std::istream& operator>>(std::istream& is, bench_god& god) {
            for (auto& h : god._workbenches) {
                is >> h._bench;
                // std::cerr << "[LOG] BENCH LOC " << h._bench.pos().x << " " << h._bench.pos().y << " INPUTED"
                //   << std::endl;
            }
            return is;
        }

        std::optional<task> get_task(const point& pos) noexcept {
            double              max_eff = -1;
            std::optional<task> res;
            for (auto& h : _workbenches) {
                for (auto& t : h._tasks) {
                    task tmp(h._bench.info(), t._target->info());
                    if (std::find_if(_used_tasks.begin(), _used_tasks.end(),
                                     [&](const task& cur) {
                                         return cur == tmp  // used
                                                || (cur.sell == t._target->info() && cur.buy.type == h._bench.type());
                                     })
                            == _used_tasks.end()
                        && h._bench.produced() && !(*t._target)[h._bench.type()]) {

                        if (double eff = (price_on_sell(h._bench.type()) - price_on_buy(h._bench.type()))
                                         / (h._bench.pos().distance(t._target->pos()) + pos.distance(h._bench.pos()));
                            eff > max_eff) {
                            max_eff = eff;
                            res     = tmp;
                        }
                    }
                }
            }

            // auto& h   = _workbenches[_indeces.find(pos)->second];
            // auto  res = std::find_if(h._tasks.begin(), h._tasks.end(),
            //                          [](const edge& e) { return !e._used && !e._target->is_producing(); });
            // assert(res != h._tasks.end());
            if (res) {
                res.value().clear_up = [=] {
                    auto last = std::remove(_used_tasks.begin(), _used_tasks.end(), res);
                    std::cerr << "ERASE TARGET" << (last == _used_tasks.end()) << std::endl;
                    _used_tasks.erase(last, _used_tasks.end());
                };
                _used_tasks.push_back(res.value());
            }
            return res;
        }

#ifdef DEBUG

        workbench& get_workbench(int index) {
            return _workbenches[index]._bench;
        }
#endif

    private:
        struct edge {
            edge(workbench* target) : _target(target) {}

            workbench* _target;
        };

        struct header {

            header(const point& pos, int type) : _bench(pos, type), _tasks() {}

            workbench         _bench;
            std::vector<edge> _tasks;
        };

        std::vector<header> _workbenches;
        std::vector<task>   _used_tasks;
    };
}  // namespace msc