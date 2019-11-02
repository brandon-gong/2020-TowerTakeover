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

using namespace vex;

/**
 * Defines a few global constants for motor and sensor ports.
 * All references to motor ports throughout the code should reference these
 * definitions, so that code modifications should be minimal if we rewire
 * or switch ports for any of the motors or sensors.
 *
 * @author Brandon Gong
 * @date 10-26-19
  */

#ifndef _ROBOTMAP_H_
#define _ROBOTMAP_H_

// todo fix
#define FRONT_LEFT_MOTOR_PORT PORT20
#define FRONT_RIGHT_MOTOR_PORT PORT11
#define BACK_LEFT_MOTOR_PORT PORT19
#define BACK_RIGHT_MOTOR_PORT PORT12
#define LIFT_LEFT_MOTOR_PORT PORT10
#define LIFT_RIGHT_MOTOR_PORT PORT9
#define ROLLER_LEFT_MOTOR_PORT PORT8
#define ROLLER_RIGHT_MOTOR_PORT PORT2

#endif