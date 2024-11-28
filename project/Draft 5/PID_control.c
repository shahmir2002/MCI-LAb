#include "PID_control.h"

void PID_Init(PID_t* pid, float Kp, float Ki, float Kd, float set_point) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->set_point = set_point;
    pid->previous_error = 0.0f;
    pid->integral = 0.0f;
}

float PID_Compute(PID_t* pid, float measured_value) {
    float error = pid->set_point - measured_value;
    pid->integral += error;
    float derivative = error - pid->previous_error;
    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    pid->previous_error = error;
    return output;
}
