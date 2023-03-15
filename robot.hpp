#pragma once

#include <string>

namespace msc {
    class robot {
    public:
        // Get robot
        int get_id() const;
        int get_workbench_type() const;
        int get_object_type() const;

        double get_time_coef() const;  // time coefficient
        double get_coll_coef() const;  // collision coefficient

        double get_angle_v() const;   // angular velocity
        double get_linear_v() const;  // linear velocity

        double get_direction() const;
        double get_x() const;
        double get_y() const;

        // Set robot
        void update();  // TODO

        // v - in unit m/s
        bool forward(double v);

        // w - in unit rad/s
        bool rotate(double w);

        bool buy();
        bool sell();
        bool destroy();

        // For IO
        std::string get_state() const;

    private:
        int _id;

        int _workbench_type;
        int _object_type;

        double _time_coef;  // time coefficient
        double _coll_coef;  // collision coefficient

        double _angle_v;   // angular velocity
        double _linear_v;  // linear velocity

        double _direction;
        double _x;
        double _y;

        std::string _state;
    };
}  // namespace msc