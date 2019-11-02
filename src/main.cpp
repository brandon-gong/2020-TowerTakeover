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
#include "subsystems/MecanumDriveTank.h"
#include "subsystems/RD4BLift.h"
#include "subsystems/RollerIntake.h"

using namespace vex;

#define IS_COMPETITION 1

// Convenience preprocessor defs for wrapping controller inputs in lambda functions
// Need this to pass inputs to subsystems
#define AxisInput(x)   ([&]() -> int32_t {return joystick.x.position();})
#define ButtonInput(y) ([&]() -> bool    {return joystick.y.pressing();})

Subsystem* subsystems[3];

// Global controller instance.
controller joystick = controller(primary);
competition Competition;

// Convert the L1 and R1 bumpers (button inputs) into one axis input.
int32_t bumperAxisInput() {
  int32_t power = 75;
  // if both buttons are pressed, no power is supplied. if both are pressed, then they cancel out
  if(joystick.ButtonUp.pressing() == joystick.ButtonDown.pressing()) return 0;
  else if(joystick.ButtonUp.pressing()) return +power;
  else return -power + 30;
};

void teleop() {
  // Continuously update all of the subsystems in the `subsystems` array
  while(true) {
    for(Subsystem* subsystem : subsystems) subsystem->update();
    wait(25, msec);
  }
}

void auton() {
  task::sleep(100);
}

/**
 * Main entry point of the code.
 *
 * Initialize all subsystems we need (e.g. drivetrain, lift, intake), then continuously
 * update them until the program is interrupted.
 */
int main() {

  // Initialize all of the subsystems.
  subsystems[0] = 
    new RD4BLift(
      bumperAxisInput, // didn't have enough axes to work with, so this is bumper L1 and R1
      LIFT_LEFT_MOTOR_PORT,
      LIFT_RIGHT_MOTOR_PORT
    );
  subsystems[1] = 
    new RollerIntake(
      ButtonInput(ButtonR1),
      ButtonInput(ButtonR2),
      ROLLER_LEFT_MOTOR_PORT,
      ROLLER_RIGHT_MOTOR_PORT
    );
  subsystems[2] = 
    new MecanumDriveTank(
      AxisInput(Axis3),
      AxisInput(Axis2),
      AxisInput(Axis1),
      FRONT_RIGHT_MOTOR_PORT,
      FRONT_LEFT_MOTOR_PORT,
      BACK_RIGHT_MOTOR_PORT,
      BACK_LEFT_MOTOR_PORT
    );

#if IS_COMPETITION
  Competition.autonomous(auton);
  Competition.drivercontrol(teleop);

  task::sleep(100);

  while(1) {
    task::sleep(100);
  }
#else
  teleop();
#endif

  return 0;
}
