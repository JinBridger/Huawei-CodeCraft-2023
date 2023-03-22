#pragma once

#include "core.hpp"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace msc {
    struct point {
        point(double x = 0.0, double y = 0.0) noexcept : x(x), y(y) {}
        point(const point& p) noexcept : x(p.x), y(p.y) {}
        point(point&& p) noexcept : x(p.x), y(p.y) {}

        double distance(const point& p) const noexcept {
            return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
        }

        double arctan(const point& p) const noexcept {
            return atan((p.y - y) / (p.x - x));
        }

        friend std::istream& operator>>(std::istream& is, point& p) {
            return is >> p.x >> p.y;
        }
        friend std::ostream& operator<<(std::ostream& os, const point& p) {
            return os << p.x << p.y;
        }
        point& operator=(const point& p) noexcept {
            if (std::addressof(p) != this)
                x = p.x, y = p.y;
            return *this;
        }
        point& operator=(point&& p) noexcept {
            return *this = p;
        }

        [[nodiscard]] friend bool operator==(const point& lhs, const point& rhs) noexcept {
            return fabs(lhs.x - rhs.x) < 1e-6 && fabs(lhs.y - rhs.y);
        }

        [[nodiscard]] friend bool operator!=(const point& lhs, const point& rhs) noexcept {
            return !(lhs == rhs);
        }

        double x, y;
    };

    struct task {
        point                 sell, buy;
        std::function<void()> clear_up;
    };

    class workbench {
    public:
        workbench(const point& p, int type)
            : _pos(p), _type(type), _left_time(0), _material_state(0), _product_state() {}

        // bool operator[](size_t idx) {
        //     return _material_state[idx];
        // }

        friend std::istream& operator>>(std::istream& is, workbench& ben) {
            return is >> ben._type >> ben._pos >> ben._left_time >> ben._material_state >> ben._product_state;
        }

        int type() const noexcept {
            return _type;
        }

        bool produced() const noexcept {
            return _product_state;
        }

        const point& pos() const noexcept {
            return _pos;
        }

    private:
        int _type;

        point _pos;

        int  _left_time;  // in unit frame
        int  _material_state;
        bool _product_state;
    };

    // TODO: A heuristic function to calculate profit of a new task
    // This is from a purchasable workbench.
    inline double task_profit(const workbench& src, const workbench& dest) {
        assert(can_product(src.type()));
        return price_on_sell(src.type()) - price_on_buy(src.type());
    }

    // TODO: A heuristic function to calculate profit when the robot completes a task and is ready to start a new one
    // This is from a consumable workbench.
    // Typically, it equals TASK-PROFIT - DISTANCE-COST
    constexpr double next_task_profit() {
        return 0.0;
    }

    class robot {
    public:
        robot(int id) : _action_status(Idle), _task_status(Waiting), _id(id), _task() {}

        bool is_waiting() const noexcept {
            return _task_status == Waiting;
        }

        bool is_busy() const noexcept {
            return _action_status > 0;
        }

        void start_task(const task& t) {
            // Called by scheduler to start a task
            std::cerr << "[LOG] CALLED START_TASK" << std::endl;
            if (fabs(t.sell.x) < 1e-6)
                return;
            _task = t;

            std::cerr << "[LOG] Robot " << _id << " Buy: [" << _task.buy.x << ", " << _task.buy.y << "]\tSell: ["
                      << _task.sell.x << ", " << _task.sell.y << "]" << std::endl;
            _task_status = Buying;
            start_action<Buy>(_task.sell);
        }

        void continue_task() {
            // Call by scheduler to continueu the unfinished task
            if (is_busy())
                continue_action();
            else {
                if (_task_status == Buying) {
                    _task_status = Selling;
                    _target      = _task.sell;
                    start_action<Sell>(_task.buy);
                }
                else if (_task_status == Selling) {
                    _task_status = Waiting;
                    if (_task.clear_up) {
                        std::cerr << "[LOG] CALLED_CLEAR_UP" << std::endl;
                        _task.clear_up();
                    }
                }
            }
        }

        friend std::istream& operator>>(std::istream& is, robot& r) {
            is >> r._workbench >> r._item >> r._time_coef >> r._coll_coef >> r._angle_v;
            double vx, vy;
            is >> vx >> vy;
            r._linear_v = sqrt(vx * vx + vy * vy);
            // std::cerr << "[LOG] ROBOT " << r._id << " INPUT VX: " << vx << " VY: " << vy << std::endl;
            return is >> r._direction >> r._pos;
        }

        void update_pos(const point& p) noexcept {
            _pos = p;
        }

        std::string get_state() {
            return std::exchange(_state, "");
        }

        point pos() const {
            return _pos;
        }

    private:
        enum ActionStatus { Idle, Buy, Sell };
        enum TaskStatus { Waiting, Buying, Selling };

        template <ActionStatus status>
        void start_action(const point& target) {
            // Set action
            _target = target;
            if constexpr (status == Sell)
                _action_status = Sell;
            else
                _action_status = Buy;

            // std::cerr << "[LOG] Robot " << _id << " now going to [" << _target_x << ", " << _target_y << "]"
            //   << std::endl;

            continue_action();
        }

        void continue_action() {
            // Continue the unfinished action

            // Calculate the target angle of workbench
            double target_angle = _pos.arctan(_target);

            // copysign()
            if (_target.x < _pos.x)
                target_angle += copysign(PI, _target.y - _pos.y);

            // Calculate the angle between robot and workbench
            double delta_angle = target_angle - _direction;
            if (delta_angle < -PI)
                delta_angle += 2 * PI;

            if (delta_angle > PI)
                delta_angle -= 2 * PI;

            // Calculate the distance between robot and workbench
            double delta_dis = _pos.distance(_target);

            // std::cerr << "ACTION STATUS  " << _action_status << std::endl;
            if (is_busy() && delta_dis < 0.4) {
                if (_action_status == Buy) {
                    buy();
                }
                if (_action_status == Sell) {
                    sell();
                }
                forward(0);
                rotate(0);
                // std::cerr << "[LOG] ROBOT X: " << _x << "\tY: " << _y << std::endl;
                // std::cerr << "[LOG] TARGT X: " << _target_x << "\tY: " << _target_y << std::endl;
                // std::cerr << "[LOG] Robot " << _id << " dis is " << delta_dis << std::endl;
                // std::cerr << "[LOG] Robot " << _id << " set to idle" << std::endl;

                _action_status = Idle;
                return;
            }
            if (is_busy()) {
                // Correction the direction
                if (fabs(delta_angle) > get_angle_threshold(delta_dis, fabs(delta_angle))) {
                    if (delta_angle > 0)
                        rotate(PI);
                    else
                        rotate(-PI);
                }
                else {
                    rotate(0);
                }

                // Set Speed
                forward(get_speed_threshold(delta_dis, fabs(delta_angle)));
            }
        }

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
            _state += "buy " + std::to_string(_id) + "\n";
            return true;
        }
        bool sell() {
            _state += "sell " + std::to_string(_id) + "\n";
            return true;
        }
        bool destroy() {
            _state += "destroy " + std::to_string(_id) + "\n";
            return true;
        }

        int _id;

        int _workbench;
        int _item;

        double _time_coef;  // time coefficient
        double _coll_coef;  // collision coefficient

        double _angle_v;   // angular velocity
        double _linear_v;  // linear velocity

        double _direction;
        point  _pos;

        point _target;

        task _task;

        ActionStatus _action_status;
        TaskStatus   _task_status;

        std::string _state;
    };
}  // namespace msc
