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

#include "subsystems/RD4BLift.h"

/*
 * Assign all of the constructor parameters to the private internal variables,
 * either via direct assignment or the copy constructor.
 * Delegates to the more thorough but longer `RD4BLift` constructor.
 */

RD4BLift::RD4BLift(AxisInput axisInput, int32_t leftMotorPort, int32_t rightMotorPort):
    liftMotor0(leftMotorPort),
    liftMotor1(rightMotorPort) {
      this->manualInput = axisInput;
      this->groundInput = [](){return false;};
      this->lowerTowerInput = [](){return false;};
      this->upperTowerInput = [](){return false;};
      this->liftMotor0.resetRotation();
      this->liftMotor0.resetPosition();
      this->liftMotor1.resetRotation();
      this->liftMotor1.resetPosition();
      this->liftMotor0.setBrake(brakeType::brake);
      this->liftMotor1.setBrake(brakeType::brake);
      this->state = State::MANUAL;
  }


/*
 * Assign all of the constructor parameters to the private internal variables,
 * either via direct assignment or the copy constructor.
 */
RD4BLift::RD4BLift( AxisInput manualInput,
            ButtonInput groundInput,
            ButtonInput lowerTowerInput,
            ButtonInput upperTowerInput,
            int32_t leftMotorPort,
            int32_t rightMotorPort ):
  liftMotor0(leftMotorPort),
  liftMotor1(rightMotorPort) {
  this->manualInput = manualInput;
  this->groundInput = groundInput;
  this->lowerTowerInput = lowerTowerInput;
  this->upperTowerInput = upperTowerInput;
  this->state = State::MANUAL;

  // Don't know what the difference is, docs say the same thing, just using both in case
  this->liftMotor0.resetRotation();
  this->liftMotor0.resetPosition();
  this->liftMotor1.resetRotation();
  this->liftMotor1.resetPosition();
  this->liftMotor0.setBrake(brakeType::brake);
  this->liftMotor1.setBrake(brakeType::brake);
}

/*
 * Called once per tick, `update()` updates the state based on user input,
 * then calls the corresponding state function that actually moves the motors
 * on the lift accordingly.
 */
void RD4BLift::update() {

  // manualInput MUST be the first thing on this conditional chain because
  // it has to take higher precedence over the automatic features.

  // If there is input, switch to the state the input is for
  if(abs(this->manualInput()) > _RD4BLIFT_H_DBAND) {
    this->state = State::MANUAL;
  } else if(this->groundInput()) {
    this->state = State::GROUND;
  } else if(this->lowerTowerInput()) {
    this->state = State::LOWER_TOWER;
  } else if(this->upperTowerInput()) {
    this->state = State::UPPER_TOWER;
  }

  // Then execute the corresponding state function
  switch(this->state) {
    case State::MANUAL:      this->stateManual();
                             break;
    case State::GROUND:      this->stateGround();
                             break;
    case State::LOWER_TOWER: this->stateLowerTower();
                             break;
    case State::UPPER_TOWER: this->stateUpperTower();
                             break;
  }
}

/*
 * Manually control the lift, moving it up and down by motor percentage.
 * Hopefully shouldn't use this much during competition, but it will always be
 * here as a safety and fallback feature.
 */
void RD4BLift::stateManual() {
  // TODO scale inputs??
  //int32_t input = (abs(this->manualInput()) > _RD4BLIFT_H_DBAND) ? this->manualInput() : 0;

  // Safety features: check to make sure the lift position isn't outside of the correct range;
  // if it is, only allow it to move in the direction back to the correct range.
  // if(this->liftMotor0.position(rotationUnits::raw) >= _RD4BLIFT_H_MAX_HEIGHT && input > 0) {
  //   this->liftMotor0.stop(brakeType::brake);
  //   this->liftMotor1.stop(brakeType::brake);
  //   return;
  // } else if(this->liftMotor0.position(rotationUnits::raw) <= 0 && input < 0) {
  //   this->liftMotor0.stop(brakeType::brake);
  //   this->liftMotor1.stop(brakeType::brake);
  //   return;
  // }

  // Then pretty much directly apply the input to the motors as percent output.
  this->liftMotor0.setVelocity(this->manualInput(), percent);
  this->liftMotor1.setVelocity(-1 * this->manualInput(), percent);
  this->liftMotor0.spin(forward);
  this->liftMotor1.spin(forward);
}

/*
 * Set the `RD4BLift` position to the lowermost position, and hold it there.
 */
void RD4BLift::stateGround() {
  this->liftMotor0.startRotateTo(_RD4BLIFT_H_FLOOR, rotationUnits::rev);
  this->liftMotor1.startRotateTo(_RD4BLIFT_H_FLOOR, rotationUnits::rev);
  // this->liftMotor0.setVelocity(20, percent);
  // this->liftMotor1.setVelocity(20, percent);
  // this->liftMotor0.spin(forward);
  // this->liftMotor1.spin(forward);
}

/*
 * Set the `RD4BLift` position to the lower tower position, and hold it there.
 */
void RD4BLift::stateLowerTower() {
  this->liftMotor0.startRotateTo(_RD4BLIFT_H_LOWER_TOWER, rotationUnits::rev);
  this->liftMotor1.startRotateTo(_RD4BLIFT_H_LOWER_TOWER, rotationUnits::rev);
  // this->liftMotor0.setVelocity(40, percent);
  // this->liftMotor1.setVelocity(40, percent);
  // this->liftMotor0.spin(forward);
  // this->liftMotor1.spin(forward);
}

/*
 * Set the `RD4BLift` position to the upper tower position, and hold it there.
 */
void RD4BLift::stateUpperTower() {
  // this->liftMotor0.startRotateTo(_RD4BLIFT_H_UPPER_TOWER, rotationUnits::raw);
  // this->liftMotor1.startRotateTo(_RD4BLIFT_H_UPPER_TOWER, rotationUnits::raw);
  this->liftMotor0.setVelocity(60, percent);
  this->liftMotor1.setVelocity(60, percent);
  this->liftMotor0.spin(forward);
  this->liftMotor1.spin(forward);
}
