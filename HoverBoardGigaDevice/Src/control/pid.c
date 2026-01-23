#include "../Inc/control/pid.h"

void PID_Init(PID_Controller *pid, float p, float i, float d, float dt, float maxOut) {
    pid->Kp = p;
    pid->Ki = i;
    pid->Kd = d;
    pid->dt = dt;
    pid->maxOutput = maxOut;
    pid->maxIntegral = maxOut * 0.5f; // Limit całki na 50% mocy // TODO: Dlaczego?
    PID_Reset(pid);
}

void PID_Reset(PID_Controller *pid) {
    pid->errorSum = 0.0f;
}

int32_t PID_Compute(PID_Controller *pid, float targetAngle, float measuredAngle, float gyroRate) {
    float error = targetAngle - measuredAngle;

    // --- 1. Proportional (P) ---
    float pTerm = pid->Kp * error;

    // --- 2. Integral (I) ---
    pid->errorSum += error * pid->dt;
    
    // Anti-windup (przycinanie całki)
    if (pid->errorSum > pid->maxIntegral) pid->errorSum = pid->maxIntegral;
    else if (pid->errorSum < -pid->maxIntegral) pid->errorSum = -pid->maxIntegral;
    
    float iTerm = pid->Ki * pid->errorSum;

    // --- 3. Derivative (D) ---
    // Używamy "Derivative on Measurement" (bezpośrednio Gyro),
    // co zapobiega szarpnięciom (Derivative Kick).
    // Odejmujemy, bo D ma przeciwdziałać zmianom.
    float dTerm = pid->Kd * gyroRate;

    // --- 4. Suma ---
    float output = pTerm + iTerm - dTerm;

    // --- 5. Limit wyjścia (Clamp) ---
    if (output > pid->maxOutput) output = pid->maxOutput;
    else if (output < -pid->maxOutput) output = -pid->maxOutput;

    return (int32_t)output;
}