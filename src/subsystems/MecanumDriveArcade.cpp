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

#include "subsystems/MecanumDriveArcade.h"

/*
 * Assign all of the constructor parameters to the private internal variables,
 * either via direct assignment or the copy constructor.
 * TODO: if we do anything autonomous, encoders should be reset here
 */
MecanumDriveArcade::MecanumDriveArcade(AxisInput drive, AxisInput strafe, AxisInput twist,
                           int32_t fr, int32_t fl, int32_t br, int32_t bl) :
  frontRight(fr),
  frontLeft(fl),
  backRight(br),
  backLeft(bl) {
  this->driveAxis = drive;
  this->strafeAxis = strafe;
  this->twistAxis = twist;
};

/*
 * Assign all of the constructor parameters to the private internal variables,
 * either via direct assignment or the copy constructor.
 * TODO: if we do anything autonomous, encoders should be reset here
 */
MecanumDriveArcade::MecanumDriveArcade(AxisInput inputs[3], motor motors[4]) :
  frontRight(motors[0]),
  frontLeft(motors[1]),
  backRight(motors[2]),
  backLeft(motors[3]) {
  driveAxis = inputs[0];
  strafeAxis = inputs[1];
  twistAxis = inputs[2];
};

/*
 * Called once per tick
 */
void MecanumDriveArcade::update() {

  // Get input values from the AxisInput functions for all 3 axes,
  // inverting if necessary
  int32_t drivePower  = this->driveAxis();
  int32_t strafePower = this->strafeAxis();
  int32_t twistPower  = this->twistAxis();

  // Apply deadband (zero out input if it is less than the deadband)
  drivePower  = (abs(drivePower) > _MDA_H_DEADBAND) ? drivePower : 0;
  strafePower = (abs(strafePower) > _MDA_H_DEADBAND) ? strafePower : 0;
  twistPower  = (abs(twistPower) > _MDA_H_DEADBAND) ? twistPower : 0;

  // Apply cubic interpolation if needed.  TODO: tweak so the deadband doesn't grow as well
  if(_MDA_H_CUBIC_DRIVE) {
    // Scale input down from -100...100 to -1...1
    double scaled = (double) drivePower / 100;
    // cube it
    scaled =  scaled *  scaled *  scaled;
    // Scale it back up from -1...1 to -100...100,
    // truncating off the rest of the decimal spots.
    drivePower = (int32_t)  (scaled * 100);
  }
  // Same process for strafe and twist
  if(_MDA_H_CUBIC_STRAFE) {
    double scaled = (double) strafePower / 100;
    scaled =  scaled *  scaled *  scaled;
    strafePower = (int32_t)  (scaled * 100);
  }
  if(_MDA_H_CUBIC_TWIST) {
    double scaled = (double) twistPower / 100;
    scaled =  scaled *  scaled *  scaled;
    twistPower = (int32_t)  (scaled * 100);
  }

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
