#include "servo.h"
#include "esp_log.h"

static const char* TAG = "Servo";
servo_t* servo_create(ledc_channel_t channel, ledc_mode_t mode, int min_angle, int max_angle, 
                       float (*angle_to_duty_ratio)(int angle), uint32_t timer_freq) {
  servo_t* servo = (servo_t*)malloc(sizeof(servo_t));
  if (servo == NULL) {
    ESP_LOGE(TAG, "Failed to allocate memory for servo");
    return NULL;
  }

  servo->channel = channel;
  servo->mode = mode;
  servo->min_angle = min_angle;
  servo->max_angle = max_angle;
  if (angle_to_duty_ratio == NULL) {
    ESP_LOGE(TAG, "Invalid angle_to_duty_ratio function");
    free(servo);
    return NULL;
  }
  servo->angle_to_duty_ratio = angle_to_duty_ratio;
  servo->timer_freq = timer_freq;
  return servo;
}

void servo_attach(servo_t* servo) {
  if (servo == NULL) {
    ESP_LOGE(TAG, "Invalid servo pointer");
    return;
  }

  ledc_config_t ledc_config;
  ledc_config.duty_resolution = LEDC_TIMER_13_BIT;
  ledc_config.freq_hz = servo->timer_freq;  // Use specified timer frequency
  ledc_config.speed_mode = servo->mode;
  ledc_config.clk_cfg = LEDC_AUTO_CLK;
  ledc_config.gpio_sel = (ledc_channel_config_t){
   .channel = servo->channel,
   .duty = 0,
   .gpio_num = -1, // Let hardware pick a free GPIO pin
   .intr_type = LEDC_INTR_DISABLE,
   .duty_cycle = 0,
   .hpoint = 0,
  };
  if (ledc_config_channel(servo->channel, &ledc_config)!= ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure LEDC channel");
  }
}

void servo_detach(servo_t* servo) {
  if (servo == NULL) {
    ESP_LOGE(TAG, "Invalid servo pointer");
    return;
  }

  ledc_stop(servo->channel, servo->mode);
  free(servo);
}

void servo_write(servo_t* servo, int angle) {
  if (servo == NULL) {
    ESP_LOGE(TAG, "Invalid servo pointer");
    return;
  }

  // Clip angle to valid range
  angle = constrain(angle, servo->min_angle, servo->max_angle);

  // Calculate duty cycle based on angle
  float duty_ratio = servo->angle_to_duty_ratio(angle);

  // Convert duty ratio to LEDC units
  uint32_t duty = (duty_ratio * ((1 << LEDC_TIMER_13_BIT) - 1));

  // Set LEDC duty cycle
  ledc_set_duty(servo->mode, servo->channel, duty);
  ledc_update_duty(servo->mode, servo->channel);
}

void servo_writeMicroseconds(servo_t* servo, int pulse_width) {
  if (servo == NULL) {
    ESP_LOGE(TAG, "Invalid servo pointer");
    return;
  }

  // Clip pulse width to valid range
  pulse_width = constrain(pulse_width, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

  // Calculate duty cycle from pulse width and frequency
  float duty_ratio = (float)pulse_width / (1000000.0f / servo->timer_freq);

  // Convert duty ratio to LEDC units
  uint32_t duty = (duty_ratio * ((1 << LEDC_TIMER_13_BIT) - 1));

  // Set LEDC duty cycle
  ledc_set_duty(servo->mode, servo->channel, duty);
  ledc_update_duty(servo->mode, servo->channel);
}

float linear_angle_to_duty_ratio(int angle) {
  // Adjust this function based on your servo motor's characteristics
  return (angle - 0) / (180.0f - 0.0f);
}
