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

#ifndef _MECDRIVE_H_
#define _MECDRIVE_H_

// Defines whether to convert inputs into motor percent outputs in a linear (false)
// or cubic (true) manner.  Use cubic for more sensitivity at lower speeds.
#define _MECDRIVE_H_CUBIC_DRIVE false
#define _MECDRIVE_H_CUBIC_STRAFE false
#define _MECDRIVE_H_CUBIC_TWIST true

// Defines a ring around the center of the joystick where inputs are ignored
#define _MECDRIVE_H_DEADBAND 5

/**
 * Defines a subsystem for controlling a Mecanum drive base.
 *
 * @author Brandon Gong
 * @date 10-26-19
 */
class MecanumDrive : public Subsystem {

  public:

    /**
     * Let the mecanum drive base update with new input values.
     * This should be called once per tick for smooth control.
     */
    void update() override;

    /**
     * Creates a new instance of the Mecanum Drive subsystem.
     *
     * @param
     *    drive - A function which returns the position of the drive joystick axis.
     *    strafe - A function which returns the position of the strafe joystick axis.
     *    twist - A function which returns the position of the twist joystick axis.
     *    fr - The port number of the front-right motor.
     *    fl - The port number of the front-left motor.
     *    br - The port number of the back-right motor.
     *    bl - The port number of the back-left motor.
     */
    MecanumDrive( AxisInput drive,
                  AxisInput strafe,
                  AxisInput twist,
                  int32_t fr,
                  int32_t fl,
                  int32_t br,
                  int32_t bl );

    /**
     * Creates a new instance of the Mecanum Drive subsystem.
     *
     * @param
     *    inputs - An array of 3 functions that return the positions of the drive,
     *             strafe, and twist axes, respectively.
     *    motors - An array of 4 vex::motor that represent the front-right, front-left,
     *             back-right, and back-left motors of the drive base, respectively.
     */
    MecanumDrive(AxisInput inputs[3], motor motors[4]);

  private:
    // Internal variables for inputs and motors.
    AxisInput driveAxis, strafeAxis, twistAxis;
    motor frontRight, frontLeft, backRight, backLeft;
};

#endif
