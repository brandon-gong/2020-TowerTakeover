<img src="readmeicon.png" align="left" height=150 />

# Team 87528A Tower Takeover
[![Build Status](https://travis-ci.org/brandon-gong/2020-TowerTakeover.svg?branch=master)](https://travis-ci.org/brandon-gong/2020-TowerTakeover)
![Version](https://img.shields.io/badge/version-v0.5.0-informational.svg)

This is Team 87528A Code Ninjas' code for the 2019-2020 VRC game,
Tower Takeover.

<br>

There are two key strengths arising from the way the code is structured and
implemented:
- **Modularity.** Instead of cramming all of the program logic into
  one `while()` loop in `main()`, it is divided into classes specific to
  the subsystem that the code relates to.  This allows us to develop code
  in terms of specific *subsystems* instead of the entire *robot*.  Subsystem
  code is kept independent of motors and controller inputs.  As a result,
  it allows us to reuse these components, simply picking the subsystems we
  need, instantiating them with the desired controller inputs and motor
  outputs, and updating them in a loop.
- **Statefulness.** Every Subsystem *must* implement an `update()` function,
  which is called repeatedly (once per tick).  This partiularly allows for
  Subsystems to take on _states_ and switch between states readily and
  autonomously, without need for logic within `main()`.  For example, a lift
  subsystem may have a manual control state, in which the user may control the
  lift with a joystick; when the user hits a button, it automatically switches
  to a state in which the lift holds itself at a certain height; however this
  state can be easily canceled and switched back to manual control at any
  given time.  So, overall, we have modularization of the robot code by subsystem;
  within a subsystem, we have modularization of code by functionality.  This
  grants the software an unusual level of flexibility, robustness, and
  reusability.

The code is structured as follows:

## `include/`
Contains header files for all of the code.  All classes and functions
are defined and documented in these files.

### `RobotMap.h`
This file contains preprocessor definitions for constants such as motor
and sensor ports which are then referenced throughout the code,
especially in `main.cpp` where subsystems are initialized.  In this way,
it is very easy for anyone to modify the code if they change the ports
for any reason, because it is all conveniently placed in one location.

### `vex.h`
This file contains `include`s that are used throughout the code, such as
`<stdlib.h>`, `"v5.h"`, and `"RobotMap.h"`.  This is just a convenient
way to reduce the amount of includes we need to have in each file; however,
it should be modified only sparingly, if a *specific* include is used 
*throughout* the file.  Otherwise, it is often clearer and more transparent
to simply include it in the file it is used in.

### `subsystems/`
#### `Subsystem.h`
Abstract class that defines methods that *all* subsystems must implement.
Again, this class should be modified *very* sparingly, because changes to
this file means changes to the code in all subsystems.  This class is
critical to the modular, stateful nature of the robot code.

#### `RD4BLift.h`
Implements `Subsystem.h`.  This defines functions and member variables for
operating a reverse double 4-bar lift.  `RD4BLift` is _stateful_, featuring
a state for manual control as well as states for lifting to the various
often-used heights (lower tower, upper tower, ground).  It also has safety
features to ensure the lift does not extend beyond its intended range of
operation.

#### `MecanumDrive.h`
Implements `Subsystem.h`.  This defines functions and member variables for
operating a mecanum drive.  It is pretty plain so far, but robot-oriented
and field-oriented control states may be implemented in the future.
Additionally, if autonomous or driver assistance code is ever done,
`MecanumDrive.h` will have to be updated to account for this.

## `src/`
Contains implementations for all the header files as well as `main.cpp`.

### `main.cpp`
The entry point of execution of the code, `main.cpp` initializes new
instances of the subsystems with the specified ports and controls and
then updates them repeatedly in a loop during robot operation.
To maintain the modular design of the code, `main.cpp` should contain
_minimal_ actual logic; the logic should be separated and kept in either
the related subsystem file or in a new subsystem of its own.  `main.cpp`
is only responsible for instantiating and updating.

### `subsystems/`
Contains `.cpp` files corresponding to that of the `include/subsystems/`
folder that implement the header subsystem files.  Documentation in these
files is not as related to how to use the code as it is to the
behind-the-scenes details of the code.  Documentation for how to include
and use the code is placed in the header file.
