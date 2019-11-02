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

#include "subsystems/RollerIntake.h"

RollerIntake::RollerIntake( ButtonInput inInput,
                            ButtonInput outInput,
                            int32_t leftMotorPort,
                            int32_t rightMotorPort ):
  left(leftMotorPort),
  right(rightMotorPort) {
  this->inInput = inInput;
  this->outInput = outInput;
  this->left.setBrake(brakeType::hold);
  this->right.setBrake(brakeType::hold);
}

void RollerIntake::update() {
  // if both buttons are pressed, no power is supplied. if both are pressed, then they cancel out
  if(inInput() == outInput()) {
    this->right.setVelocity(0, percent);
    this->left.setVelocity(0, percent);
    this->right.spin(forward);
    this->left.spin(forward);
  } else if(outInput()) {
    this->right.setVelocity(_ROLLER_H_POWER, percent);
    this->left.setVelocity(-1 * _ROLLER_H_POWER, percent);
    this->right.spin(forward);
    this->left.spin(forward);
  } else {
    this->right.setVelocity(-1 * _ROLLER_H_POWER, percent);
    this->left.setVelocity(_ROLLER_H_POWER, percent);
    this->right.spin(forward);
    this->left.spin(forward);
  }
}