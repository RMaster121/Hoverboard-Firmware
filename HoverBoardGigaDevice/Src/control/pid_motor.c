// #include "../Inc/control/pid.h"

// void PID_Init(PID_Controller *pid, float p, float i, float d, float dt, float maxOut) {
//     pid->Kp = p;
//     pid->Ki = i;
//     pid->Kd = d;
//     pid->dt = dt;
//     pid->maxOutput = maxOut;
//     pid->maxIntegral = maxOut * 0.5f; // Limit całki na 50% mocy // TODO: Dlaczego?
//     PID_Reset(pid);
// }

// void PID_Reset(PID_Controller *pid) {
//     pid->errorSum = 0.0f;
// }

// int32_t PID_Compute(PID_Controller *pid, float targetAngle, float measuredAngle, float gyroRate) {
//     float error = targetAngle - measuredAngle;

//     // --- 1. Proportional (P) ---
//     float pTerm = pid->Kp * error;

//     // --- 2. Integral (I) ---
//     pid->errorSum += error * pid->dt;
    
//     // Anti-windup (przycinanie całki)
//     if (pid->errorSum > pid->maxIntegral) pid->errorSum = pid->maxIntegral;
//     else if (pid->errorSum < -pid->maxIntegral) pid->errorSum = -pid->maxIntegral;
    
//     float iTerm = pid->Ki * pid->errorSum;

//     // --- 3. Derivative (D) ---
//     // Używamy "Derivative on Measurement" (bezpośrednio Gyro),
//     // co zapobiega szarpnięciom (Derivative Kick).
//     // Odejmujemy, bo D ma przeciwdziałać zmianom.
//     float dTerm = pid->Kd * gyroRate;

//     // --- 4. Suma ---
//     float output = pTerm + iTerm - dTerm;

//     // --- 5. Limit wyjścia (Clamp) ---
//     if (output > pid->maxOutput) output = pid->maxOutput;
//     else if (output < -pid->maxOutput) output = -pid->maxOutput;

//     return (int32_t)output;
// }




// //NEW NEW NEW
// // --- DOSTOSOWANIE DO TWOJEGO KODU ---

// #include "control/pid.h"

// // Funkcja pomocnicza do przycinania wartości (Clamp)
// static float clamp(float value, float min, float max) {
//     if (value > max) return max;
//     if (value < min) return min;
//     return value;
// }

// void PID_Init(PID_Controller *pid, float p, float i, float d, float dt, float maxOut, float maxInteg) {
//     pid->Kp = p;
//     pid->Ki = i;
//     pid->Kd = d;
//     pid->dt = dt;
//     pid->maxOutput = maxOut;
//     pid->maxIntegral = maxInteg;
    
//     PID_Reset(pid);
// }

// void PID_Reset(PID_Controller *pid) {
//     pid->integralSum = 0.0f;
//     pid->prevMeasurement = 0.0f;
// }

// // --- WERSJA DO BALANSU (Z Żyroskopem) ---
// // Używa "Derivative on Measurement" z zewnętrznego źródła (Gyro)
// // To eliminuje "Derivative Kick" przy zmianie Setpointu
// float PID_Compute_Gyro(PID_Controller *pid, float setpoint, float measurement, float rateInput) {
//     // 1. Uchyb (Error)
//     float error = setpoint - measurement;

//     // 2. Człon Proporcjonalny (P)
//     float P_term = pid->Kp * error;

//     // 3. Człon Całkujący (I)
//     pid->integralSum += (error * pid->dt);

//     // Anti-Windup dla całki
//     pid->integralSum = clamp(pid->integralSum, -pid->maxIntegral, pid->maxIntegral);
//     float I_term = pid->Ki * pid->integralSum;

//     // 4. Człon Różniczkujący (D)
//     // UWAGA: Odejmujemy, ponieważ gdy kąt rośnie, chcemy kontrować.
//     // rateInput to zazwyczaj Gyro (prędkość zmian).
//     float D_term = -pid->Kd * rateInput;

//     // 5. Suma i Limit wyjścia
//     float output = P_term + I_term + D_term;
//     output = clamp(output, -pid->maxOutput, pid->maxOutput);

//     return output;
// }

// // --- WERSJA DO SILNIKÓW (Pętla Prądowa) ---
// // Sama liczy pochodną na podstawie zmiany pomiaru
// float PID_Compute_Standard(PID_Controller *pid, float setpoint, float measurement) {
//     // 1. Uchyb
//     float error = setpoint - measurement;

//     // 2. P
//     float P_term = pid->Kp * error;

//     // 3. I
//     pid->integralSum += (error * pid->dt);
//     pid->integralSum = clamp(pid->integralSum, -pid->maxIntegral, pid->maxIntegral);
//     float I_term = pid->Ki * pid->integralSum;

//     // 4. D (Derivative on Measurement) - obliczane matematycznie
//     // (Obecny - Poprzedni) / Czas
//     float derivative = (measurement - pid->prevMeasurement) / pid->dt;
//     float D_term = -pid->Kd * derivative;

//     // Zapamiętaj pomiar do następnego cyklu
//     pid->prevMeasurement = measurement;

//     // 5. Wyjście
//     float output = P_term + I_term + D_term;
//     output = clamp(output, -pid->maxOutput, pid->maxOutput);

//     return output;
// }