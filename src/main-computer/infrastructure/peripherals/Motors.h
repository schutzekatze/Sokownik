/*
 * Motors.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef INFRASTRUCTURE_PERIPHERALS_MOTORS_H
#define INFRASTRUCTURE_PERIPHERALS_MOTORS_H

#include <tuple>
using std::tuple;

#include <thread>
using std::thread;

#include <mutex>
using std::mutex;

#include <condition_variable>
using std::condition_variable;

#include <cmath>
using std::cbrt;
using std::fabs;

namespace infrastructure {

// Threadsafe
class Motors {

public:

    /*
     * Set and get powers given to the robot motors.
     * The values range from -1.0 to 1.0, with -1.0 meaning
     * going reverse at max speed, and 1.0 going forward at max speed.
     *
     * The power is applied over time, instead of instantly, to prevent
     * drifting. This can be configured with the FULL_SPEEDUP_TIME constant,
     * which defines the time required to go from 0.0 to 1.0 speed.
     */
    static void set_powers(float power_left, float power_right);
    static tuple<float, float> get_powers();

private:

    static constexpr int   FULL_SPEEDUP_TIME = 2000; // milliseconds
    static constexpr int   POWER_CHANGE_PERIOD = 100; // milliseconds
    static constexpr float DEGREE_STEP = ((float)POWER_CHANGE_PERIOD) / FULL_SPEEDUP_TIME;
    static constexpr float CONVERSION_FACTOR = 255.0; // Computer power to microcontroller power

    Motors() = delete;

    static float power_left, power_right; // Current powers
    static float power_left_desired, power_right_desired; // Desired powers
    static mutex power_mutex;
    static bool power_desired_changed;
    static condition_variable power_desired_changed_condition;

    // Linearly changing power degree; maps to power
    static float degree_left, degree_right; // Current degrees
    static float degree_left_desired, degree_right_desired; // Desired degrees
    static float degree_left_step_sign, degree_right_step_sign; // +1 or -1, depending on the change

    static thread power_regulator;
    static void power_regulator_routine();

    static int sgn(float n) {
        return n >= 0 ? 1 : -1;
    }

    static float degree2power(float degree) {
        if ((degree < 0.05) && (degree > -0.05))
            return 0;
        else
            return degree * degree * degree * 0.7 + sgn(degree) * 0.3;
    }

    static float power2degree(float power) {
        if ((power < 0.3) && (power > -0.3))
            return 0;
        else
            return cbrt((power - sgn(power) * 0.3) / 0.7);
    }

};

}  /* namespace infrastructure */

#endif /* INFRASTRUCTURE_PERIPHERALS_MOTORS_H */
