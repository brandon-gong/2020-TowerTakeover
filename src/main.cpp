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

#include "vex.h"
#include "subsystems/MecanumDrive.h"
#include "subsystems/RD4BLift.h"

using namespace vex;

// Convenience preprocessor defs for wrapping controller inputs in lambda functions
// Need this to pass inputs to subsystems
#define AxisInput(x)   ([&]() -> int32_t {return joystick.x.position();})
#define ButtonInput(y) ([&]() -> bool    {return joystick.y.pressing();})

// Global controller instance.
controller joystick = controller(primary);

// Convert the L1 and R1 bumpers (button inputs) into one axis input.
int32_t bumperAxisInput() {
  int32_t power = 80;
  // if both buttons are pressed, no power is supplied. if both are pressed, then they cancel out
  if(joystick.ButtonL1.pressing() == joystick.ButtonR1.pressing()) return 0;
  else if(joystick.ButtonL1.pressing()) return +power;
  else return -power;
};

/**
 * Main entry point of the code.
 *
 * Initialize all subsystems we need (e.g. drivetrain, lift, intake), then continuously
 * update them until the program is interrupted.
 */
int main() {

  // Initialize all of the subsystems.
  Subsystem* subsystems[] = {
    new MecanumDrive(
      AxisInput(Axis3), // left joystick up & down
      AxisInput(Axis4), // left joystick right & left
      AxisInput(Axis1), // right joystick right & left
      FRONT_RIGHT_MOTOR_PORT,
      FRONT_LEFT_MOTOR_PORT,
      BACK_RIGHT_MOTOR_PORT,
      BACK_LEFT_MOTOR_PORT),
    new RD4BLift(
      bumperAxisInput, // didn't have enough axes to work with, so this is bumper L1 and R1
      ButtonInput(ButtonB), // B
      ButtonInput(ButtonY), // Y
      ButtonInput(ButtonX), // X
      LIFT_LEFT_MOTOR_PORT,
      LIFT_RIGHT_MOTOR_PORT
    )
  };

  // Continuously update all of the subsystems in the `subsystems` array
  while(true) {
    for(Subsystem* subsystem : subsystems) subsystem->update();
    wait(25, msec);
  }

  return 0;
}
