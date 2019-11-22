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

// TODO clean competition junk
motor fl(FRONT_LEFT_MOTOR_PORT),
      fr(FRONT_RIGHT_MOTOR_PORT),
      bl(BACK_LEFT_MOTOR_PORT),
      br(BACK_RIGHT_MOTOR_PORT),
      il(ROLLER_LEFT_MOTOR_PORT),
      ir(ROLLER_RIGHT_MOTOR_PORT);


// Convert the up and down inputs (button inputs) into one axis input.
int32_t updownAxisInput() {
  int32_t power = 75;
  // if no buttons are pressed, no power is supplied. if both are pressed, then they cancel out
  if(joystick.ButtonL1.pressing() == joystick.ButtonL2.pressing()) return 0;
  else if(joystick.ButtonL1.pressing()) return +power;
  else return -power + 30;
};

// Convert the up and down inputs (button inputs) into one axis input.
int32_t yaAxisInput() {
  int32_t power = 75;
  // if no buttons are pressed, no power is supplied. if both are pressed, then they cancel out
  if(joystick.ButtonA.pressing() == joystick.ButtonY.pressing()) return 0;
  else if(joystick.ButtonA.pressing()) return +power;
  else return -power;
};

void teleop() {
  // Continuously update all of the subsystems in the `subsystems` array
  while(true) {
    for(Subsystem* subsystem : subsystems) subsystem->update();
    wait(25, msec);
  }
}

// todo clean
void auton() {
  fl.setVelocity(-50, percent);
  fr.setVelocity(50, percent);
  bl.setVelocity(-50, percent);
  br.setVelocity(50, percent);
  fl.spin(forward);
  fr.spin(forward);
  bl.spin(forward);
  br.spin(forward);
  task::sleep(2000);
  fl.stop(brakeType::brake);
  fr.stop(brakeType::brake);
  bl.stop(brakeType::brake);
  br.stop(brakeType::brake);
  il.setVelocity(-50, percent);
  ir.setVelocity(50, percent);
  fl.setVelocity(20, percent);
  fr.setVelocity(-20, percent);
  bl.setVelocity(20, percent);
  br.setVelocity(-20, percent);
  fl.spin(forward);
  fr.spin(forward);
  bl.spin(forward);
  br.spin(forward);
  ir.spin(forward);
  il.spin(forward);
  task::sleep(3000);
  fl.stop(brakeType::brake);
  fr.stop(brakeType::brake);
  bl.stop(brakeType::brake);
  br.stop(brakeType::brake);
  il.stop(brakeType::brake);
  ir.stop(brakeType::brake);
  task::sleep(300);
  fl.setVelocity(-50, percent);
  fr.setVelocity(50, percent);
  bl.setVelocity(-50, percent);
  br.setVelocity(50, percent);
  fl.spin(forward);
  fr.spin(forward);
  bl.spin(forward);
  br.spin(forward);
  task::sleep(1250);
  fl.stop(brakeType::brake);
  fr.stop(brakeType::brake);
  bl.stop(brakeType::brake);
  br.stop(brakeType::brake);
  task::sleep(300);
  fl.setVelocity(50, percent);
  fr.setVelocity(-50, percent);
  bl.setVelocity(50, percent);
  br.setVelocity(-50, percent);
  fl.spin(forward);
  fr.spin(forward);
  bl.spin(forward);
  br.spin(forward);
  task::sleep(3000);
  fl.stop(brakeType::brake);
  fr.stop(brakeType::brake);
  bl.stop(brakeType::brake);
  br.stop(brakeType::brake);
  task::sleep(300);
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
      updownAxisInput, // didn't have enough axes to work with, so this is bumper L1 and R1
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
      yaAxisInput,
      ButtonInput(ButtonB),
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
