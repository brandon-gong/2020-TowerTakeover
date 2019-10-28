/*
 * Copyright (c) 2019 Brandon Gong
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Subsystem.h"

#ifndef _RD4BLIFT_H_
#define _RD4BLIFT_H_

/**
 * Defines a deadband for the lift, a zone in which small inputs will be ignored
 */
#define _RD4BLIFT_H_DBAND 5

/*
 * Defines encoder values, measured in raw units, of the min/max positions of the lift,
 * the lower tower, and the upper tower.  These may need to be tuned periodically.
 */
#define _RD4BLIFT_H_MAX_HEIGHT 1000
#define _RD4BLIFT_H_FLOOR 0
#define _RD4BLIFT_H_LOWER_TOWER 300
#define _RD4BLIFT_H_UPPER_TOWER 700

/**
 * Defines a subsystem for controlling an RD4B lift.
 * This subsystem is _stateful_, meaning it has a variety of different operation modes
 * which can be readily be switched between.
 *
 * A given instance of `RD4BLift` is in one of four states at any given time:
 *  - State::MANUAL = Manual control through the input axis supplied during instantiation
 *  - State::GROUND = `RD4BLift` position is at lowest level.
 *  - State::LOWER_TOWER = `RD4BLift` position is held to the lower tower height by encoder.
 *  - State::UPPER_TOWER = `RD4BLift` position is held to the upper tower height by encoder.
 *
 * As a safety feature, State::MANUAL always take precedence over the other automated states;
 * i.e. automated movements can always be immediately cancelled, even when not yet completed,
 * by any manual input to the axis.
 *
 * State::MANUAL also includes additional safety measures that ensure the lift is never
 * extended beyond its intended range of motion.
 *
 * @author Brandon Gong
 * @date 10-26-19
 */
class RD4BLift : public Subsystem {

  public:

    /**
     * Let the RD4B lift update with new input values.
     * This *must* be called once per tick.  The robot can and will be damaged otherwise.
     */
    void update() override;

    /**
     * Creates a new instance of `RD4BLift`, without any button inputs.
     * This will essentially disable all states besides State::MANUAL.
     *
     * @param
     *    axisInput - a function that when called returns some ranged input.
     *    leftMotorPort - The port on the Brain that the left motor is plugged in to.
     *    rightMotorPort - The port on the Brain that the right motor is plugged in to.
     *
     * @delegate Lift::Lift(AxisInput, ButtonInput, ButtonInput, ButtonInput, int32_t, int32_t)
     */
    RD4BLift(AxisInput axisInput, int32_t leftMotorPort, int32_t rightMotorPort);

    /**
     * Creates a new instance of `RD4BLift`.
     *
     * @param
     *    manualInput - a function that when called returns some ranged input.
     *    groundInput - a function that returns a boolean for toggling State::GROUND.
     *    lowerTowerInput - a function that returns a boolean for toggling State::LOWER_TOWER.
     *    upperTowerInput - a function that returns a boolean for toggling State::UPPER_TOWER.
     *    leftMotorPort - The port on the Brain that the left motor is plugged in to.
     *    rightMotorPort - The port on the Brain that the right motor is plugged in to.
     */
    RD4BLift( AxisInput manualInput,
              ButtonInput groundInput,
              ButtonInput lowerTowerInput,
              ButtonInput upperTowerInput,
              int32_t leftMotorPort,
              int32_t rightMotorPort );

  private:

    /**
     * Defines possible States that `RD4BLift` can be in.
     */
    enum State {
      MANUAL,       // Manual control via axis input
      GROUND,       // Held at ground level
      LOWER_TOWER,  // Held at lower tower level
      UPPER_TOWER   // Held at upper tower level
    };

    // Current state of this `RD4BLift` instance.
    State state;

    // Functions that correspond to a certain state, and are called by update() based on state.
    void stateManual();     // State::MANUAL
    void stateGround();     // State::GROUND
    void stateLowerTower(); // State::LOWER_TOWER
    void stateUpperTower(); // State::UPPER_TOWER

    // Internal variables for storing all of the functions for obtaining user input and the two motors.
    AxisInput manualInput;
    ButtonInput groundInput, lowerTowerInput, upperTowerInput;
    motor liftMotor0, liftMotor1;
};

#endif
