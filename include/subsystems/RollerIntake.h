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

#ifndef _ROLLER_H_
#define _ROLLER_H_

#define _ROLLER_H_POWER 75

/**
 * Defines a subsystem for controlling a roller intake.
 * Assumes one motor on each side and constant intake/outtake speeds.
 * This may be modified to take an axis later on.
 *
 * @author Brandon Gong
 * @date 11-1-19
 */
class RollerIntake : public Subsystem {

  public:
  
    /**
     * Let the roller intake update with new input values from the two buttons.
     * This should be called once per tick for smooth control.
     */
    void update() override;

    /**
     * Create a new instance of roller intake.
     */
    RollerIntake( ButtonInput inInput,
                  ButtonInput outInput,
                  int32_t leftMotorPort,
                  int32_t rightMotorPort );

  private:

    ButtonInput inInput, outInput;
    motor left, right;

};

#endif
