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

#include "subsystems/MecanumDriveTank.h"

/*
 * Assign all of the constructor parameters to the private internal variables,
 * either via direct assignment or the copy constructor.
 * TODO: if we do anything autonomous, encoders should be reset here
 */
MecanumDriveTank::MecanumDriveTank(AxisInput lDrive, AxisInput rDrive, AxisInput strafe,
                           int32_t fr, int32_t fl, int32_t br, int32_t bl) :
  frontRight(fr),
  frontLeft(fl),
  backRight(br),
  backLeft(bl) {
  this->lDriveAxis = lDrive;
  this->rDriveAxis = rDrive;
  this->strafeAxis = strafe;
  this->frontRight.setBrake(brakeType::coast);
  this->frontLeft.setBrake(brakeType::coast);
  this->backRight.setBrake(brakeType::coast);
  this->backLeft.setBrake(brakeType::coast);
};

/*
 * Assign all of the constructor parameters to the private internal variables,
 * either via direct assignment or the copy constructor.
 * TODO: if we do anything autonomous, encoders should be reset here
 */
MecanumDriveTank::MecanumDriveTank(AxisInput inputs[3], motor motors[4]) :
  frontRight(motors[0]),
  frontLeft(motors[1]),
  backRight(motors[2]),
  backLeft(motors[3]) {
  lDriveAxis = inputs[0];
  rDriveAxis = inputs[1];
  strafeAxis = inputs[2];
  this->frontRight.setBrake(brakeType::coast);
  this->frontLeft.setBrake(brakeType::coast);
  this->backRight.setBrake(brakeType::coast);
  this->backLeft.setBrake(brakeType::coast);
};

void MecanumDriveTank::update() {

  // Get input values from the AxisInput functions for all 3 axes,
  // inverting if necessary
  int32_t lDrivePower  = this->lDriveAxis();
  int32_t rDrivePower  = -this->rDriveAxis();
  int32_t strafePower = this->strafeAxis();

  // Apply deadband (zero out input if it is less than the deadband)
  lDrivePower  = (abs(lDrivePower) > _MDT_H_DEADBAND) ? lDrivePower : 0;
  rDrivePower  = (abs(rDrivePower) > _MDT_H_DEADBAND) ? rDrivePower : 0;
  strafePower = (abs(strafePower) > _MDT_H_DEADBAND) ? strafePower : 0;

  int32_t drivePower = (lDrivePower + rDrivePower) * 0.5;
  int32_t twistPower = (lDrivePower - rDrivePower) * 0.5;

  // Calculate the power of each motor based on the drive, strafe, and twist powers,
  // in the order front-left, front-right, back-left, back-right.
  int32_t motorPowers[] = {
    drivePower + twistPower + strafePower,
    drivePower - twistPower - strafePower,
    drivePower + twistPower - strafePower,
    drivePower - twistPower + strafePower
  };

  // Normalize to -100...100, only if the maximum of the 4 motor speeds is outside
  // of that range.
  double maxPower = 0;
  for(int32_t motorPower : motorPowers) {
    if(abs(motorPower) > maxPower) maxPower = abs(motorPower);
  }
  maxPower /= 100;
  if(maxPower > 1) {
    for(int i = 0; i < 4; i++) {
      motorPowers[i] = (int32_t) (motorPowers[i] / maxPower);
    }
  }

  // Set calculated speeds on the motors.
  this->frontLeft.setVelocity(motorPowers[0], percent);
  this->frontRight.setVelocity(motorPowers[1], percent);
  this->backLeft.setVelocity(motorPowers[2], percent);
  this->backRight.setVelocity(motorPowers[3], percent);

  // Spin.
  this->frontLeft.spin(forward);
  this->frontRight.spin(forward);
  this->backLeft.spin(forward);
  this->backRight.spin(forward);
}