//-----------------------------------------------------------------------------
/*

Timer Functions

The main stepper routines use timers and ISRs for pulse generation.
This file has the board specific routines to allow this.

*/
//-----------------------------------------------------------------------------

#ifndef TIMERS_H
#define TIMERS_H

//-----------------------------------------------------------------------------
// step timing control

// The highest timer rate is (120/2) = 60MHz
// We divide it by 4 to give a counter rate of 15 MHz.
// ie 1 us == 15
#define TICKS_PER_MICROSECOND 15

#define LASER_TIMER_PERIOD 255
//-----------------------------------------------------------------------------

void step_isr_enable(void);
void step_isr_disable(void);

void set_step_period(uint32_t ticks);
void set_step_pulse_delay(uint32_t ticks);
void set_step_pulse_time(uint32_t ticks);

// interrupt context callbacks to generate step/direction pulses
void step_pulse_isr(void);
void step_delay_isr(void);
void step_period_isr(void);

//-----------------------------------------------------------------------------
void control_laser_intensity(uint8_t intensity);  //0-255 is 0-100%
void control_laser_pwm(uint8_t intensity);
//-----------------------------------------------------------------------------

// cdc interface
void cdc_timer_isr(void);
void cdc_timer_start(void);
void cdc_timer_stop(void);

//-----------------------------------------------------------------------------
void timers_init(void);

//-----------------------------------------------------------------------------

#endif // TIMERS_H

//-----------------------------------------------------------------------------
