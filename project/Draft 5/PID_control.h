#ifndef PID_CONTROL_H
#define PID_CONTROL_H

typedef struct {
    float Kp;  // Proportional gain
    float Ki;  // Integral gain
    float Kd;  // Derivative gain
    float previous_error; // Previous error value
    float integral;       // Integral of the error
    float set_point;      // Desired distance or target speed
} PID_t;

void PID_Init(PID_t* pid, float Kp, float Ki, float Kd, float set_point);
float PID_Compute(PID_t* pid, float measured_value);

#endif // PID_CONTROL_H
