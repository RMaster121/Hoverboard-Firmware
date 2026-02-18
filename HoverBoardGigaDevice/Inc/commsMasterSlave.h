#ifndef COMMS_MASTER_SLAVE_H
#define COMMS_MASTER_SLAVE_H

#include <stdint.h>
#include "../Inc/defines.h" 

#define COMM_START_BYTE 0xAA

#pragma pack(1)
// Struktura rozkazów (Master -> Slave)
typedef struct {
    uint8_t start;
    int16_t target_rpm;
    uint8_t flags;
    uint8_t checksum;
} PacketM2S;

// Struktura telemetrii (Slave -> Master)
typedef struct {
    uint8_t start;
    int16_t pitch;      // Kąt * 100
    int16_t actual_rpm;
    uint8_t error_code;
    uint8_t checksum;
} PacketS2M;

#pragma pack()

extern uint8_t usart1_rx_buf[128]; //TODO: zmienna do define

#ifdef MASTER
    #define SerialReceive PacketS2M
    #define SerialSend PacketM2S
#else
    #define SerialReceive PacketM2S
    #define SerialSend PacketS2M
#endif

/**
 * Initialization of communication module
 */
void Comms_Init(void);

/**
 * Process incoming transmission - triggered by DMA interrupt
 */
void ProcessMasterSlaveRx(uint32_t rx_write_pos);

#ifdef MASTER
    void SendToSlave(int16_t rpm, uint8_t flags);
#else
    void SendToMaster(int16_t pitch, int16_t rpm, uint8_t err);
#endif

#endif