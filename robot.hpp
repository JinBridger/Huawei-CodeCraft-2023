#pragma once

#include <cmath>
#include <string>

namespace msc {
    class robot {
    public:
        // Get robot
        int get_id() const {
            return _id;
        }
        int get_workbench_type() const {
            return _workbench_type;
        }
        int get_object_type() const {
            return _object_type;
        }

        double get_time_coef() const {  // time coefficient
            return _time_coef;
        }
        double get_coll_coef() const {  // collision coefficient
            return _coll_coef;
        }

        double get_angle_v() const {  // angular velocity
            return _angle_v;
        }
        double get_linear_v() const {  // linear velocity
            return _linear_v;
        }

        double get_direction() const {
            return _direction;
        }
        double get_x() const {
            return _x;
        }
        double get_y() const {
            return _y;
        }

        // Set robot
        void update(int id, int workbench_type, int object_type, double time_coef, double coll_coef, double angle_v,
                    double linear_vx, double linear_vy, double direction, double x, double y) {
            _id = id;

            _workbench_type = workbench_type;
            _object_type    = object_type;

            _time_coef = time_coef;  // time coefficient
            _coll_coef = coll_coef;  // collision coefficient

            _angle_v   = angle_v;    // angular velocity
            _linear_vx = linear_vx;  // x linear velocity
            _linear_vy = linear_vy;  // y linear velocity

            _direction = direction;
            _x         = x;
            _y         = y;

            _linear_v = sqrt(linear_vx * linear_vx + linear_vy * linear_vy);  // linear velocity
            _state    = "";
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

        // For IO
        std::string get_state() {
            std::string ret_val = _state;

            _state = "";
            return ret_val;
        }

    private:
        int _id;

        int _workbench_type;
        int _object_type;

        double _time_coef;  // time coefficient
        double _coll_coef;  // collision coefficient

        double _angle_v;    // angular velocity
        double _linear_v;   // linear velocity
        double _linear_vx;  // x linear velocity
        double _linear_vy;  // y linear velocity

        double _direction;
        double _x;
        double _y;

        std::string _state;
    };
}  // namespace msc