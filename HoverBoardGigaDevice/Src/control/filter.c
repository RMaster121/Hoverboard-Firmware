#include "../Inc/control/filter.h"

void Filter_Init(ComplementaryFilter *filter, float alpha, float dt) {
    filter->currentAngle = 0.0f;
    filter->alpha = alpha;
    filter->dt = dt;
    filter->isInitialized = 0;
}

float Filter_CalculateAngle(ComplementaryFilter *filter, int16_t ax_raw, int16_t az_raw, int16_t gy_raw) {
    // 1. Oblicz kąt z akcelerometru (ArcTan2)
    // Używamy ay i az (zakładając, że MPU leży płasko, a oś y to oś obrotu kół)
    float accAngle = atan2f((float)ax_raw, -(float)az_raw) * RAD_TO_DEG;

    // 2. Przelicz żyroskop na prędkość kątową (deg/s)
    float gyroRate = (float)gy_raw / 65.6f;

    // Jeśli to pierwszy pomiar, nie używamy wzoru rekurencyjnego,
    // tylko ufamy w 100% akcelerometrowi.
    if (filter->isInitialized == 0) {
        filter->currentAngle = accAngle;
        filter->isInitialized = 1; // Zaznacz, że filtr już wystartował
        return filter->currentAngle;
    }

    // 3. Wzór filtru komplementarnego
    // Kąt = 0.9 * (Poprzedni + Gyro * czas) + 0.02 * Akcelerometr
    filter->currentAngle = filter->alpha * (filter->currentAngle + gyroRate * filter->dt) 
                         + (1.0f - filter->alpha) * accAngle;

    return filter->currentAngle;
}