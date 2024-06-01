#ifndef SERVO_H
#define SERVO_H

#include <driver/ledc.h>

#define MIN_PULSE_WIDTH 500  // Minimum pulse width for servo (in microseconds)
#define MAX_PULSE_WIDTH 2500 // Maximum pulse width for servo (in microseconds)

typedef struct {
  ledc_channel_t channel;
  ledc_mode_t mode;
  int min_angle;
  int max_angle;
  float (*angle_to_duty_ratio)(int angle);
  uint32_t timer_freq;  // PWM timer frequency (in Hz)
} servo_t;

servo_t* servo_create(ledc_channel_t channel, ledc_mode_t mode, int min_angle, int max_angle, 
                       float (*angle_to_duty_ratio)(int angle), uint32_t timer_freq = 50);
void servo_attach(servo_t* servo);
void servo_detach(servo_t* servo);
void servo_write(servo_t* servo, int angle);
void servo_writeMicroseconds(servo_t* servo, int pulse_width); // Similar to Arduino writeMicroseconds

#endif
