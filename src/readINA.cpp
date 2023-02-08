#include <readINA.h>

namespace readINA
{
    INA233 ina(INA233_ADDRESS_41, Wire);
    float rShunt = 0.004;
    float iMax = 5.0;
    
    Comms::Packet p = {.id = 1};

    void init()
    {
        // let'sa gooo!
        // Serial.begin(921600); Comms takes care of this
        DEBUG("Initializing INA...");

        //Wire.setClock(400000); 
        //Wire.setPins(1,2); These ain't my problem.
        //Wire.begin();

        ina.init(rShunt,iMax);
    }

    void readAndSend()
    {
        // read the ina
        float busVoltage = ina.readBusVoltage();
        float shuntCurrent = ina.readCurrent();
        //float shuntVoltage = ina.readShuntVoltage(); don't need this
        float power = ina.readPower();
        //float avgPower = ina.readAvgPower(); eh maybe?

        //make Packet
        p.len = 0;
        Comms::packetAddFloat(&p, busVoltage);
        Comms::packetAddFloat(&p, shuntCurrent);
        Comms::packetAddFloat(&p, power);

        // emit the packet
        Comms::emitPacket(&p);
    }

    void print()
    {
        // read the ina
        float busVoltage = ina.readBusVoltage();
        float shuntCurrent = ina.readCurrent();
        //float shuntVoltage = ina.readShuntVoltage(); don't need this
        float power = ina.readPower();
        //float avgPower = ina.readAvgPower(); eh maybe?

        // print the ina
        Serial.print("Bus Voltage: ");
        Serial.println(busVoltage);
        Serial.print("Shunt Current: ");
        Serial.println(shuntCurrent);
        Serial.print("Power: ");
        Serial.println(power);
    }
}