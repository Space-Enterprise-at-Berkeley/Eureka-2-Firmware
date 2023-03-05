#include "HAL.h"

namespace HAL {

    int DRVSPISpeed = 1000000;
    int ADCSPISpeed = 5000000;
    volatile bool motorDriverFault = false;
    uint8_t SPIBUFF[2]; // spi buffer for all SPI except ethernet.
    SPIClass *motorSPI = NULL;
    SPIClass *dataSPI = NULL;
    volatile int encoderTicks = 0;
    
    double cumPhaseCurrentA = 0;
    double cumPhaseCurrentB = 0;
    double cumPhaseCurrentC = 0;
    int32_t numReads = 0;

    int openLimitSwitchEngaged = 0;
    int closedLimitSwitchEngaged = 0;

    uint32_t motorOvercurrentStartTime;
    float overcurrentThreshold = 50; //amps
    uint32_t overcurrentHysteresis = 100; //milliseconds

    bool inOvercurrentCooldown = false;
    uint32_t overcurrentSafingStartTime;
    uint32_t overcurrentRetryTime = 1000;

    bool motorDriverEnabled;

    const uint32_t OCBufferSize = 10;
    float overCurrentBuffer[OCBufferSize];
    uint32_t overCurrentBufferPtr = 0;

    int init() {


        // motorSPI = new SPIClass(HSPI);
        dataSPI = new SPIClass(FSPI);
        


        // motorSPI->begin(MOTOR_SCLK, MOTOR_MISO, MOTOR_MOSI);
        // pinMode(MADC_CS, OUTPUT);
        // digitalWrite(MADC_CS, HIGH);
        pinMode(PTADC_CS, OUTPUT);
        digitalWrite(PTADC_CS, HIGH);
        dataSPI->begin(ETH_SCLK, ETH_MISO, ETH_MOSI);

        setupEncoder();

        // int motorDriverInitSuccess = initializeMotorDriver();
        // if (motorDriverInitSuccess == -1) {
        //     disableMotorDriver();
        //     return -1;
        // }

        int pwmFreq = 50;
        int pwmResolution = 12;
        ledcSetup(motorChannel, pwmFreq, pwmResolution);
        ledcAttachPin(SPARKMAX, 0);
        ledcWrite(motorChannel, 307);

        // pinMode(LIMIT_1, INPUT);
        // pinMode(LIMIT_2, INPUT);
        // closedLimitSwitchEngaged = digitalRead(LIMIT_1);
        // openLimitSwitchEngaged = digitalRead(LIMIT_2);
        // attachInterrupt(LIMIT_1, valveClosedLimitSwitchTrigger, CHANGE);
        // attachInterrupt(LIMIT_2, valveOpenLimitSwitchTrigger, CHANGE);

        pinMode(TEMPSENSE0, INPUT);
        pinMode(TEMPSENSE1, INPUT);

        return 0;
    }

    
    int initializeMotorDriver() {
        Serial.printf("motor driver init start\n");
        pinMode(DRV_CS, OUTPUT);
        pinMode(DRV_EN, OUTPUT);
        pinMode(DRV_FAULT, INPUT_PULLUP);
        pinMode(INHC, OUTPUT);
        pinMode(INLC, OUTPUT);

        disableMotorDriver();
        motorDriverFault = false;
        digitalWrite(DRV_CS, HIGH);
        digitalWrite(INHC, LOW);
        digitalWrite(INLC, LOW);
        delay(10);


        int pwmFreq = 50000;
        int pwmResolution = 8;
        ledcSetup(motorChannel, pwmFreq, pwmResolution);
        ledcAttachPin(INHA, 0);
        ledcWrite(motorChannel, 0);

        enableMotorDriver();
        delay(10);
        printMotorDriverFaultAndDisable();
        enableMotorDriver();
        delay(10);
        attachInterrupt(DRV_FAULT, handleMotorDriverFault, FALLING);

        //set driver control
        SPIBUFF[0] = 0b00000000;
        SPIBUFF[1] = 0b11000000; //1x pwm control for trap control
        writeMotorDriverRegister(2);

        //set CSA
        SPIBUFF[0] = 0b00000010;
        SPIBUFF[1] = 0b00000001; //sense ocp 0.5v, gain 5v/v //TODO change this
        writeMotorDriverRegister(6);


        //set gate drive LS
        SPIBUFF[0] = 0b00000111;
        SPIBUFF[1] = 0b11101110; //1000mA, 2000mA source/sink gate current
        writeMotorDriverRegister(4);

        //set OCP
        SPIBUFF[0] = 0b00000001;
        SPIBUFF[1] = 0b01110101; // OC to auto retry under fault, OC deglitch to 8us, Vds trip to 0.45V
        writeMotorDriverRegister(5);

        // set gate drive HS 
        SPIBUFF[0] = 0b00000011;
        SPIBUFF[1] = 0b11101110; //1000mA, 2000mA source/sink gate current
        writeMotorDriverRegister(3);

        readMotorDriverRegister(2);
        if ((SPIBUFF[0] != (uint8_t) 0x00) || (SPIBUFF[1] != (uint8_t) 0xc0)) {
            Serial.printf("reg 2 bad :( %hhx, %hhx\n", SPIBUFF[0], SPIBUFF[1]);
            return -1;
        } else {
            Serial.printf("reg 2 good!\n");
        }
        // delay(10);

        readMotorDriverRegister(6);
        if ((SPIBUFF[0] != (uint8_t) 0x02) || (SPIBUFF[1] != (uint8_t) 0x01)) {
            Serial.printf("reg 6 bad :( %hhx, %hhx\n", SPIBUFF[0], SPIBUFF[1]);
            return -1;
        } else {
            Serial.printf("reg 6 good!\n");
        } 
        // delay(10);

        readMotorDriverRegister(3);
        if ((SPIBUFF[0] != (uint8_t) 0x03) || (SPIBUFF[1] != (uint8_t) 0xEE)) {
            Serial.printf("reg 3 bad :( %hhx, %hhx\n", SPIBUFF[0], SPIBUFF[1]);
            return -1;
        } else {
            Serial.printf("reg 3 good!\n");
        }
        // delay(10);


        readMotorDriverRegister(4);
        if ((SPIBUFF[0] != (uint8_t) 0x07) || (SPIBUFF[1] != (uint8_t) 0xEE)) {
            Serial.printf("reg 4 bad :( %hhx, %hhx\n", SPIBUFF[0], SPIBUFF[1]);
            return -1;
        } else {
            Serial.printf("reg 4 good!\n");
        }
        // printMotorDriverFaultAndDisable();

        readMotorDriverRegister(5);
        if ((SPIBUFF[0] != (uint8_t) 0x01) || (SPIBUFF[1] != (uint8_t) 0x75)) {
            Serial.printf("reg 5 bad :( %hhx, %hhx\n", SPIBUFF[0], SPIBUFF[1]);
            return -1;
        } else {
            Serial.printf("reg 5 good!\n");
        }
        delay(1);
        // disableMotorDriver(); //remove this later
        return 0;

    }

    void readMotorDriverRegister(int8_t addr) {
        SPIBUFF[0]  = 0x00;
        SPIBUFF[1] = 0x00;
        SPIBUFF[0] |= 0b10000000; //set read mode
        addr &= 0x0F;
        SPIBUFF[0] |= (addr << 3);
        sendSPICommand(SPIBUFF, 2, motorSPI, DRV_CS, DRVSPISpeed, SPI_MODE1);
        SPIBUFF[0] &= 0b00000111;
    }

    void writeMotorDriverRegister(int8_t addr) {
        SPIBUFF[0] &= 0b01111111; //set write mode
        addr &= 0x0F;
        SPIBUFF[0] |= (addr << 3);
        sendSPICommand(SPIBUFF, 2, motorSPI, DRV_CS, DRVSPISpeed, SPI_MODE1);
    }

    void sendSPICommand(uint8_t* dataBuffer, int numBytes, SPIClass* spi, int csPin, int clkSpeed, int spiMode) {
        uint8_t prevFirstByte = dataBuffer[0];
        spi->beginTransaction(SPISettings(clkSpeed, MSBFIRST, spiMode));
        spi->transfer(dataBuffer, 1);
        spi->endTransaction();
        dataBuffer[0] = prevFirstByte;
        spi->beginTransaction(SPISettings(clkSpeed, MSBFIRST, spiMode));
        digitalWrite(csPin, LOW);
        spi->transfer(dataBuffer, numBytes);
        spi->endTransaction();
        digitalWrite(csPin, HIGH);
    }

    void enableMotorDriver() {
        digitalWrite(DRV_EN, HIGH);
        motorDriverEnabled = true;
    }

    void disableMotorDriver() {
        digitalWrite(DRV_EN, LOW);
        motorDriverEnabled = false;
    }

    void handleMotorDriverFault() {
        motorDriverFault = true;
        ledcWrite(motorChannel, 0);
    }

    void printMotorDriverFaultAndDisable() {
        
        if (digitalRead(DRV_FAULT) == HIGH) {
            Serial.printf("but the fault pin is high? so ignoring\n");
            return;
        }
        ledcWrite(motorChannel, 0);
        delayMicroseconds(100);
        readMotorDriverRegister(0);
        Serial.printf("Fault:\n reg 0 <%hhx>, <%hhx>\n", SPIBUFF[0], SPIBUFF[1]);
        readMotorDriverRegister(1);
        Serial.printf("reg 1 <%hhx>, <%hhx>\n", SPIBUFF[0], SPIBUFF[1]);
        readMotorDriverRegister(0);
        if ((SPIBUFF[0] == 0x00) && (SPIBUFF[1] == 0x00)) {
            Serial.printf("but both regs are 0? so ignoring\n");
            return;
        }
    
        delayMicroseconds(10);
        disableMotorDriver();

    }

    float volt_to_motor_temp(float volt) {
        float resist = (10000*volt) / (5-volt);
        float denom = log(resist / (10000 * exp((-1*3380)/298.15)));
        float kelv = 3380 / denom;
        return kelv - 273.15;   
    }


    
    void clearMotorDriverFault() {
        motorDriverFault = false;
    }

    bool getMotorDriverFault() {
        return motorDriverFault;
    }

    void setEncoderCount(int i) {
        encoderTicks = (int) i;
    }

    int getEncoderCount() {
        return encoderTicks;
    }


    void risingA() {
        if (digitalRead(encC)) {
            encoderTicks += 1;
        } else {
            encoderTicks -= 1;
        }
    }

    void risingB() {
        if (digitalRead(encA)) {
            encoderTicks += 1;
        } else {
            encoderTicks -= 1;
        } 
    }

    void risingC() {
        if (digitalRead(encB)) {
            encoderTicks += 1;
        } else {
            encoderTicks -= 1;
        } 
    }

    void fallingA() {
        if (digitalRead(encB)) {
            encoderTicks += 1;
        } else {
            encoderTicks -= 1;
        }  
    }

    void fallingB() {
        if (digitalRead(encC)) {
            encoderTicks += 1;
        } else {
            encoderTicks -= 1;
        }  
    }

    void fallingC() {
        if (digitalRead(encA)) {
            encoderTicks += 1;
        } else {
            encoderTicks -= 1;
        }  
    }


    void setupEncoder() {
        pinMode(encA, INPUT);
        pinMode(encB, INPUT);
        pinMode(encC, INPUT);

        attachInterrupt(encA, risingA, RISING);
        attachInterrupt(encB, risingB, RISING);
        attachInterrupt(encC, risingC, RISING);

        attachInterrupt(encA, fallingA, FALLING);
        attachInterrupt(encB, fallingB, FALLING);
        attachInterrupt(encC, fallingC, FALLING);

        // Serial.printf("done setting up encoder. ticks: %d, prevEncoderState: %hhx\n", encoderTicks, prevEncoderState);

    }

 
    /**
     * monitors phase current. return true if abort.
     * @param action Desired valve state
     */
    void monitorPhaseCurrent() { 
        float phaseCurA = getPhaseCurrent(0);
        float phaseCurB = getPhaseCurrent(1);
        float phaseCurC = getPhaseCurrent(2);
        cumPhaseCurrentA += pow(phaseCurA, 2);
        cumPhaseCurrentB += pow(phaseCurB, 2);
        cumPhaseCurrentC += pow(phaseCurC, 2);
        numReads++;
        // float maxCurrent = max(max(abs(phaseCurA), abs(phaseCurB)), abs(phaseCurC));

        // overCurrentBuffer[overCurrentBufferPtr] = maxCurrent;
        // overCurrentBufferPtr = ((overCurrentBufferPtr+1) % OCBufferSize);
        // float total = 0;
        // for (int i = 0; i < OCBufferSize; i++) {
        //     total += overCurrentBuffer[i];
        // }
        // float OCThreshold = overcurrentThreshold*OCBufferSize;
        // if ((total > OCThreshold) && !inOvercurrentCooldown) {

        //     inOvercurrentCooldown = true;
        //     overcurrentSafingStartTime = millis();
            
        //     // Serial.printf("motor overcurrent.. pA: %f, pB: %f, pC: %f, avg over x samples: %f, time: %d\n", phaseCurA, phaseCurB, phaseCurC, total, millis());
        // } else {
        //     // motorOvercurrentStartTime = 0;
        //     // inOvercurrentCooldown = false;
        // } 
        // if (inOvercurrentCooldown) {
        //     if (millis() > (overcurrentSafingStartTime + overcurrentRetryTime)) {
        //         // Serial.printf("OC reset\n");
        //         inOvercurrentCooldown = false;
        //         overcurrentSafingStartTime = millis();
        //     }
        // }
    }

    void packetizePhaseCurrents(Comms::Packet* packet) {
        
        packet->len = 0;
        if (numReads == 0) {
            numReads = 1;
        }
        float curA = sqrt(cumPhaseCurrentA / (float)numReads);
        float curB = sqrt(cumPhaseCurrentB / (float)numReads);
        float curC = sqrt(cumPhaseCurrentC / (float)numReads);

        // Serial.printf("PA: %f, PB:%f, PC: %f\n", curA, curB, curC);

        float maxCurrent = max(max(abs(curA), abs(curB)), abs(curC));
        overCurrentBuffer[overCurrentBufferPtr] = maxCurrent;
        overCurrentBufferPtr = ((overCurrentBufferPtr+1) % OCBufferSize);

        float total = 0;
        for (int i = 0; i < OCBufferSize; i++) {
            total += min(overCurrentBuffer[i], overcurrentThreshold) + 1;
        }
        float totalOCThresh = overcurrentThreshold*OCBufferSize;

        if ((total > totalOCThresh) && !inOvercurrentCooldown) {

            inOvercurrentCooldown = true;
            overcurrentSafingStartTime = millis();
            
            Serial.printf("motor overcurrent.. pA: %f, pB: %f, pC: %f, avg over x samples: %f, time: %d\n", curA, curB, curC, total, millis());
        } else {
            // motorOvercurrentStartTime = 0;
            // inOvercurrentCooldown = false;
        } 
        if (inOvercurrentCooldown) {
            if (millis() > (overcurrentSafingStartTime + overcurrentRetryTime)) {
                Serial.printf("OC reset\n");
                inOvercurrentCooldown = false;
                overcurrentSafingStartTime = millis();
            }
        }

        Comms::packetAddFloat(packet, curA);
        Comms::packetAddFloat(packet, curB);
        Comms::packetAddFloat(packet, curC);
        // Serial.printf("total: %.2f, numReads: %d, time: %d\n", cumPhaseCurrentA, numReads, millis());
        cumPhaseCurrentA = 0;
        cumPhaseCurrentB = 0;
        cumPhaseCurrentC = 0;
        numReads = 0;
    }

    void valveClosedLimitSwitchTrigger() {
        closedLimitSwitchEngaged = digitalRead(LIMIT_1);
        Util::stopMotor();
    }
    void valveOpenLimitSwitchTrigger() {
        openLimitSwitchEngaged = digitalRead(LIMIT_2);
        Util::stopMotor();
    }
    int getClosedLimitSwitchState() {
        closedLimitSwitchEngaged = digitalRead(LIMIT_1);
        return closedLimitSwitchEngaged;
    }
    int getOpenLimitSwitchState() {
        openLimitSwitchEngaged = digitalRead(LIMIT_2);
        return openLimitSwitchEngaged;
    }

    void packetizeTemperatures(Comms::Packet* packet) {
        float t1 = ((((float) analogRead(TEMPSENSE0)) * (3.3 / 4096.0)) - 0.4) / (0.0195);
        float t2 = ((((float) analogRead(TEMPSENSE1)) * (3.3 / 4096.0)) - 0.4) / (0.0195);
        float motorTemp = volt_to_motor_temp(readADC(motorSPI, MADC_CS, 3));

        Comms::packetAddFloat(packet, t1);
        Comms::packetAddFloat(packet, t2);
        Comms::packetAddFloat(packet, motorTemp);
    }

    bool getOvercurrentStatus() {
        return inOvercurrentCooldown;
    }

}