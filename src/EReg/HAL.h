#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <ESP32Encoder.h>
#include "Ducers.h"
#include "Util.h" 
#include "SPI.h"
#include "Common.h"

namespace HAL {
    
    // ereg motor pins
    const int INHA = 1;
    const int INLA = 4;
    const int INHB = 2;
    const int INLB = 5;
    const int INHC = 3;
    const int INLC = 6;

    const int motorChannel = 0;


    // Best encoder Performance: both pins have interrupt capability
    // avoid using pins with LEDs attached
    //for now, only uses two encoders similar to quadrature. Less fine position control, meh
    const int encA = 14; //encoder A
    const int encB = 15; //encoder B
    const int encC = 16; //encoder C

    extern ESP32Encoder encoder;
    uint8_t SPIBUFF[2]; // spi buffer for all SPI except ethernet.
    SPIClass *motorSPI = NULL;
    SPIClass *dataSPI = NULL;
    

    const uint8_t upstreamPT = 0; // pressurant
    const uint8_t downstreamPT = 2; // tank

    const int LIMIT_1 = 40;
    const int LIMIT_2 = 41;

    const int TEMPSENSE0 = 19;
    const int TEMPSENSE1 = 20;

    const int DRV_EN = 11;
    const int DRV_FAULT = 12;

    const int DRV_CS = 10;
    const int MADC_CS = 10;
    const int ETH_CS = 10;
    const int PTADC_CS = 10;

    
    const int MOTOR_MISO = 8;
    const int MOTOR_MOSI = 9;
    const int MOTOR_SCLK = 7;

    const int ETH_MISO = 37;
    const int ETH_MOSI = 36;
    const int ETH_SCLK = 38;

    const int eregAbortID = 201;

    int DRVSPISpeed = 1000000;
    int ADCSPISpeed = 1000000;

    bool motorDriverFault = false;

    int init();
    float readUpstreamPT();
    float readDownstreamPT();
    int initializeMotorDriver();
    void sendSPICommand(void* dataBuffer, int numBytes, SPIClass* spi, int csPin, int clkSpeed, int spiMode);
    void readMotorDriverRegister(int8_t addr);
    void writeMotorDriverRegister(int8_t addr);
    void enableMotorDriver();
    void disableMotorDriver();
    void handleMotorDriverFault();
    void printMotorDriverFault();
}
