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
#include <functional>

#ifndef _SUBSYSTEM_H_
#define _SUBSYSTEM_H_

/**
 * Abstract base class which requires subclasses to define a function `update()`,
 * which is called once per tick for each subsystem.
 * This Subsystem class may be extended to include a `log()` function in the
 * future.
 *
 * @author Brandon Gong
 * @date 10-25-19
  */
class Subsystem {

  public:

    // Some convenience typedef's for axis and button inputs
    typedef std::function<int32_t()> AxisInput;
    typedef std::function<bool()> ButtonInput;

    // The `update()` function must be defined for all subsystems
    virtual void update() = 0;

};

#endif
