#include "servo.h"

// Define servo configuration (replace with your values)
ledc_channel_t channel = LEDC_CHANNEL_0;
ledc_mode_t mode = LEDC_HIGH_SPEED_MODE;
int min_angle = 0;
int max_angle = 180;
uint32_t timer_freq = 50;  // Adjust timer frequency if needed

// Create servo object
servo_t* myservo = servo_create(channel, mode, min_angle, max_angle, linear_angle_to_duty_ratio, timer_freq);
if (myservo == NULL) {
    printf("Failed to create servo object\n");
    // Handle error, perhaps by exiting the program or taking appropriate action
    return 1;
}

// Attach the servo
int attach_result = servo_attach(myservo);
if (attach_result != 0) {
    printf("Failed to attach servo: %s\n", servo_get_error_message(attach_result));
    // Handle error, perhaps by detaching the servo and exiting the program or taking appropriate action
    servo_detach(myservo);
    return 1;
}

// Control the servo
servo_write(myservo, 90);  // Set servo to 90 degrees
servo_writeMicroseconds(myservo, 1500);  // Set pulse width to 1500 microseconds

// Detach the servo when finished
int detach_result = servo_detach(myservo);
if (detach_result != 0) {
    printf("Failed to detach servo: %s\n", servo_get_error_message(detach_result));
    // Handle error, perhaps by exiting the program or taking appropriate action
    return 1;
}

return 0;
