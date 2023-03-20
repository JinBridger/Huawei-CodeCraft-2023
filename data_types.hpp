#pragma once

#include "core.hpp"

#include <bitset>
#include <iostream>
#include <string>
#include <vector>

namespace msc {
    struct workbench {
        workbench(double x, double y, int type)
            : x(x), y(y), type(type), left_time(0), material_state(), product_state(), id(0) {}

        int id;
        int type;

        double x;
        double y;

        int            left_time;  // in unit frame
        std::bitset<6> material_state;
        bool           product_state;
    };

    class bench_god {
    public:
        template <class... Ts>
        void add_bench(Ts&&... args) {
            _workbenches.emplace_back(std::forward<Ts>(args)...);
        }

        size_t size() noexcept {
            return _workbenches.size();
        }

        void update() {
            for (auto& ben : _workbenches) {
                std::cin >> ben.type;
                std::cin >> ben.x;
                std::cin >> ben.y;
                std::cin >> ben.left_time;
                std::cin >> ben.material_state;
                std::cin >> ben.product_state;
            }
        }

        // TO REMOVE
        workbench& get_workbench(int workbench_num) {
            return _workbenches[workbench_num];
        }

    private:
        std::vector<workbench> _workbenches;
    };

    class robot {
    public:
        robot(bench_god& b, int id) : _bench_god(b), _action_status(Idle), _task_status(Waiting), _id(id) {}

        bool is_idle() const {
            return (_action_status == Idle) ? true : false;
        }

        bool is_waiting() const {
            return (_task_status == Waiting) ? true : false;
        }

        void start_task(double buy_x, double buy_y, double sell_x, double sell_y) {
            // Called by scheduler to start a task
            _buy_x  = buy_x;
            _buy_y  = buy_y;
            _sell_x = sell_x;
            _sell_y = sell_y;

            std::cerr << "[LOG] Robot " << _id << " Buy: [" << buy_x << ", " << buy_y << "]\tSell: [" << sell_x << ", "
                      << sell_y << "]" << std::endl;
            _task_status = Buying;
            start_action(0, buy_x, buy_y);
        }

        void continue_task() {
            // Call by scheduler to continueu the unfinished task
            if (_action_status == Busy)
                continue_action();
            if (_action_status == Idle && _task_status == Buying) {
                _task_status = Selling;
                start_action(1, _sell_x, _sell_y);
            }
            if (_action_status == Idle && _task_status == Selling) {
                _task_status = Waiting;
            }
        }

        void update() {
            std::cin >> _workbench;
            std::cin >> _item;

            std::cin >> _time_coef;
            std::cin >> _coll_coef;

            std::cin >> _angle_v;
            double vx, vy;
            std::cin >> vx >> vy;
            _linear_v = sqrt(vx * vx + vy * vy);

            std::cin >> _direction;
            std::cin >> _x;
            std::cin >> _y;
        }

        void update_x_and_y(double x, double y) {
            _x = x;
            _y = y;
        }

        std::string get_state() {
            return std::exchange(_state, "");
        }

    private:
        void start_action(bool is_sell, double target_x, double target_y) {
            // Set action
            _target_x      = target_x;
            _target_y      = target_y;
            _action_status = Busy;
            _action_type   = is_sell ? Sell : Buy;

            // std::cerr << "[LOG] Robot " << _id << " now going to [" << _target_x << ", " << _target_y << "]"
            //   << std::endl;

            continue_action();
        }

        void continue_action() {
            // Continue the unfinished action

            // Calculate the target angle of workbench
            double dy           = _target_y - _y;
            double dx           = _target_x - _x;
            double target_angle = atan(dy / dx);
            if (dx < 0) {
                if (dy >= 0)
                    target_angle += PI;
                if (dy < 0)
                    target_angle -= PI;
            }

            // Calculate the angle between robot and workbench
            double delta_angle = target_angle - _direction;
            if (delta_angle < -PI)
                delta_angle += 2 * PI;

            if (delta_angle > PI)
                delta_angle -= 2 * PI;

            // Calculate the distance between robot and workbench
            double delta_dis = sqrt(dy * dy + dx * dx);

            if (_action_status == Busy && delta_dis < 0.4) {
                if (_action_type == Buy) {
                    buy();
                }
                if (_action_type == Sell) {
                    sell();
                }
                forward(0);
                rotate(0);
                // std::cerr << "[LOG] ROBOT X: " << _x << "\tY: " << _y << std::endl;
                // std::cerr << "[LOG] TARGT X: " << _target_x << "\tY: " << _target_y << std::endl;
                // std::cerr << "[LOG] Robot " << _id << " dis is " << delta_dis << std::endl;
                // std::cerr << "[LOG] Robot " << _id << " set to idle" << std::endl;

                _action_status = Idle;
            }
            if (_action_status == Busy) {
                // Correction the direction
                if (abs(delta_angle) > get_angle_threshold(delta_dis, abs(delta_angle))) {
                    if (delta_angle > 0)
                        rotate(PI);
                    else
                        rotate(-PI);
                }
                else {
                    rotate(0);
                }

                // Set Speed
                forward(get_speed_threshold(delta_dis, abs(delta_angle)));
            }
        }

        double _buy_x;
        double _buy_y;
        double _sell_x;
        double _sell_y;

        double get_angle_threshold(double dt_dis, double dt_angle) {
            if (dt_dis > 20)
                return 0.08;
            return 0.04;
        }

        double get_speed_threshold(double dt_dis, double dt_angle) {
            if (dt_dis > 2) {
                if (dt_angle < 0.1)
                    return 6;
                return 0;
            }
            if (dt_angle < 0.04)
                return 5;
            return 2;
        }

        enum ActionStatus { Idle, Busy };
        enum ActionType { Buy, Sell };
        enum TaskStatus { Waiting, Buying, Selling };

        bench_god& _bench_god;
        double     _target_x;
        double     _target_y;

        ActionStatus _action_status;
        ActionType   _action_type;
        TaskStatus   _task_status;

        // Low level API
        double area() const {
            return PI * pow(_item ? ROBOT_RADIUS_ON_TAKING : ROBOT_RADIUS, 2);
        }

        double mass() const {
            return ROBOT_DENSITY * area();
        }

        // v - in unit m/s
        bool forward(double v) {
            _state += "forward " + std::to_string(_id) + " " + std::to_string(v) + "\n";
            return true;
        }

        // w - in unit rad/s
        bool rotate(double w) {
            _state += "rotate " + std::to_string(_id) + " " + std::to_string(w) + "\n";
            return true;
        }

        bool buy() {
            // _state += "buy " + std::to_string(_id) + "\n";
            return true;
        }
        bool sell() {
            // _state += "sell " + std::to_string(_id) + "\n";
            return true;
        }
        bool destroy() {
            _state += "destroy " + std::to_string(_id) + "\n";
            return true;
        }

        double sell_price() const {
            return origin_sell_price(_item) * _time_coef * _coll_coef;
        }

        int _id;

        int _workbench;
        int _item;

        double _time_coef;  // time coefficient
        double _coll_coef;  // collision coefficient

        double _angle_v;   // angular velocity
        double _linear_v;  // linear velocity

        double _direction;
        double _x;
        double _y;

    private:
        std::string _state;
    };
}  // namespace msc