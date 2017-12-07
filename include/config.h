/*
  config.h - compile time configuration
  Part of LasaurGrbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c) 2011 Sungeun K. Jeon
  Copyright (c) 2011 Stefan Hechenberger

  LasaurGrbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LasaurGrbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
*/

#ifndef config_h
#define config_h

#include <inttypes.h>
#include <stdbool.h>


#define SMART_LASER_MINI    0
#define FABOOL_LASER_MINI   1
#define SMART_LASER_CO2     2
#define FABOOL_LASER_CO2    3
#define FABOOL_LASER_DS     4
#define FABOOL_LASER_MC     5

//#define GRBL_MODEL    SMART_LASER_MINI
//#define GRBL_MODEL  FABOOL_LASER_MINI
//#define GRBL_MODEL    SMART_LASER_CO2
#define GRBL_MODEL    FABOOL_LASER_CO2
//#define GRBL_MODEL    FABOOL_LASER_DS
//#define GRBL_MODEL    FABOOL_LASER_MC


#if GRBL_MODEL == SMART_LASER_MINI
    #define GRBL_STR   " SmartLaserMini"
#elif GRBL_MODEL == FABOOL_LASER_MINI
    #define GRBL_STR   " FaboolLaserMini"
#elif GRBL_MODEL == SMART_LASER_CO2
    #define GRBL_STR   " SmartLaserCo2"
#elif GRBL_MODEL == FABOOL_LASER_CO2
    #define GRBL_STR   " FaboolLaserCo2"
#elif GRBL_MODEL == FABOOL_LASER_DS
    #define GRBL_STR   " FaboolLaserDS"
#elif GRBL_MODEL == FABOOL_LASER_MC
    #define GRBL_STR   " FaboolLaserMC"
#endif


// Version number
#define GRBL_VERSION "2.10"


// I:Raster Start
// This defines the maximum number of dots in a raster.
#define RASTER_BUFFER_SIZE  128
#define RASTER_BIT_NUM      (RASTER_BUFFER_SIZE * 8)
// I:Raster End

#define CMD_FEED_HOLD '!'
#define CMD_CYCLE_START '~'

#if GRBL_MODEL == SMART_LASER_MINI
    #define CONFIG_X_STEPS_PER_MM 79.99999999 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define CONFIG_Y_STEPS_PER_MM 79.99999999 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
#elif GRBL_MODEL == FABOOL_LASER_MINI
    #define CONFIG_X_STEPS_PER_MM 53.33333333 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define CONFIG_Y_STEPS_PER_MM 53.33333333 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
#elif GRBL_MODEL == SMART_LASER_CO2
    #define CONFIG_X_STEPS_PER_MM 79.99999999 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define CONFIG_Y_STEPS_PER_MM 79.99999999 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
#elif GRBL_MODEL == FABOOL_LASER_CO2
    #define CONFIG_X_STEPS_PER_MM 53.33333333 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define CONFIG_Y_STEPS_PER_MM 53.33333333 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
#elif GRBL_MODEL == FABOOL_LASER_DS
    #define CONFIG_X_STEPS_PER_MM 53.33333333 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define CONFIG_Y_STEPS_PER_MM 53.33333333 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
#elif GRBL_MODEL == FABOOL_LASER_MC
    #define BASE_OFFSET_X (+0.0)
    #define BASE_OFFSET_Y (+0.0)
    #define BASE_ANGLE 180.0
    #define AXISLEN_A 200.0
    #define AXISLEN_B 200.0
    #define CONFIG_X_STEPS_PER_DEG 1422.22222222 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define CONFIG_Y_STEPS_PER_DEG 1422.22222222 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
    #define SVPOS_TO_RAD(p) (((double)(p) / 180.0) * M_PI)
    #define FIELD_W (575.0)
    #define X_OFS (FIELD_W/2 - 132.5)

    #define CONFIG_R1 200.0
    #define CONFIG_R2 150.0

#endif
#define CONFIG_Z_STEPS_PER_MM 44.44444444 //microsteps/mm (no integers, e.g. use 80.0 instead of 80)
#define CONFIG_PULSE_MICROSECONDS 5
#define CONFIG_FEEDRATE 8000.0 // in millimeters per minute
#define CONFIG_SEEKRATE 8000.0
#define CONFIG_ACCELERATION 1800000.0 // mm/min^2, typically 1000000-8000000, divide by (60*60) to get mm/sec^2
#define CONFIG_JUNCTION_DEVIATION 0.006 // mm
#if GRBL_MODEL == FABOOL_LASER_MC
#define CONFIG_X_ORIGIN_OFFSET 0.0  // mm, x-offset of table origin from physical home
#define CONFIG_Y_ORIGIN_OFFSET 0.0  // mm, y-offset of table origin from physical home
#else
#define CONFIG_X_ORIGIN_OFFSET 5.0  // mm, x-offset of table origin from physical home
#define CONFIG_Y_ORIGIN_OFFSET 5.0  // mm, y-offset of table origin from physical home
#endif
#define CONFIG_Z_ORIGIN_OFFSET 0.0   // mm, z-offset of table origin from physical home
#define CONFIG_INVERT_X_AXIS 1  // 0 is regular, 1 inverts the y direction
#define CONFIG_INVERT_Y_AXIS 1  // 0 is regular, 1 inverts the y direction
#define CONFIG_INVERT_Z_AXIS 1  // 0 is regular, 1 inverts the y direction

// I:Raster Start
#define CONFIG_LASER_PWM_FREQ           40000

#define CONFIG_LASER_PPI_PULSE_MS       4
#define CONFIG_LASER_PPI_SPACE_MS       1
#define CONFIG_LASER_PPI_MAX_PPM        (60000.0 / (CONFIG_LASER_PPI_PULSE_MS + CONFIG_LASER_PPI_SPACE_MS))
// I:Raster End



// The temporal resolution of the acceleration management subsystem. Higher number give smoother
// acceleration but may impact performance.
// NOTE: Increasing this parameter will help any resolution related issues, especially with machines
// requiring very high accelerations and/or very fast feedrates. In general, this will reduce the
// error between how the planner plans the motions and how the stepper program actually performs them.
// However, at some point, the resolution can be high enough, where the errors related to numerical
// round-off can be great enough to cause problems and/or it's too fast for the Arduino. The correct
// value for this parameter is machine dependent, so it's advised to set this only as high as needed.
// Approximate successful values can range from 30L to 100L or more.
#define ACCELERATION_TICKS_PER_SECOND 200L

// Minimum planner junction speed. Sets the default minimum speed the planner plans for at the end
// of the buffer and all stops. This should not be much greater than zero and should only be changed
// if unwanted behavior is observed on a user's machine when running at very slow speeds.
#define ZERO_SPEED 0.0 // (mm/min)

// Minimum stepper rate. Sets the absolute minimum stepper rate in the stepper program and never runs
// slower than this value, except when sleeping. This parameter overrides the minimum planner speed.
// This is primarily used to guarantee that the end of a movement is always reached and not stop to
// never reach its target. This parameter should always be greater than zero.
#define MINIMUM_STEPS_PER_MINUTE 800 // (steps/min) - Integer value only
// 1600 @ 32step_per_mm = 50mm/min


#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2


#define clear_vector(a) memset(a, 0, sizeof(a))
#define clear_vector_double(a) memset(a, 0.0, sizeof(a))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

// I:Raster Start
#define MM_PER_INCH (25.4)
// I:Raster End

#endif



// bit math
// see: http://www.arduino.cc/playground/Code/BitMath
// see: http://graphics.stanford.edu/~seander/bithacks.html
//
// y = (x >> n) & 1; // n=0..15. stores nth bit of x in y. y becomes 0 or 1.
//
// x &= ~(1 << n); // forces nth bit of x to be 0. all other bits left alone.
//
// x &= (1<<(n+1))-1; // leaves alone the lowest n bits of x; all higher bits set to 0.
//
// x |= (1 << n); // forces nth bit of x to be 1. all other bits left alone.
//
// x ^= (1 << n); // toggles nth bit of x. all other bits left alone.
//
// x = ~x; // toggles ALL the bits in x.

