#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <math.h>

// Stała konwersji radianów na stopnie
#define RAD_TO_DEG 57.3248407643f // 180/pi

typedef struct {
    float currentAngle;     // Aktualny kąt (wyjście filtru)
    float alpha;            // Waga żyroskopu (np. 0.98)
    float dt;               // Czas pętli w sekundach
    uint8_t isInitialized;  // FLAGA: 0 = pierwszy start, 1 = działa
} ComplementaryFilter;

/**
 * Inicjalizacja filtru.
 */
void Filter_Init(ComplementaryFilter *filter, float alpha, float dt);

/**
 * Oblicza kąt Pitch (pochylenie przód-tył).
 * gx_raw - surowe dane z żyroskopu (oś X zazwyczaj)
 * ay_raw, az_raw - surowe dane z akcelerometru
 */
float Filter_CalculateAngle(ComplementaryFilter *filter, int16_t ax_raw, int16_t az_raw, int16_t gy_raw);

#endif