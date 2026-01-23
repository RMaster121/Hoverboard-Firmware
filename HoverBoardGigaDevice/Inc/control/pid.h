#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
    // Nastawy
    float Kp;
    float Ki;
    float Kd;

    // Limity
    float dt;               // Czas próbkowania
    float maxOutput;        // Np. 1000 dla PWM
    float maxIntegral;      // Zabezpieczenie całki (anti-windup)

    // Pamięć
    float errorSum;         // Całka błędów
} PID_Controller;

/**
 * Inicjalizacja PID.
 * maxOut - maksymalna wartość PWM (np. 1000)
 */
void PID_Init(PID_Controller *pid, float p, float i, float d, float dt, float maxOut);

/**
 * Resetuje stan PID (zeruje całkę). Ważne przy restarcie silników.
 */
void PID_Reset(PID_Controller *pid);

/**
 * Główna funkcja obliczeniowa.
 * targetAngle - zazwyczaj 0.0 (pion)
 * measuredAngle - kąt z Filtru
 * gyroRate - prędkość kątowa (deg/s) - używana do członu D (Derivative on Measurement)
 */
int32_t PID_Compute(PID_Controller *pid, float targetAngle, float measuredAngle, float gyroRate);

#endif