#include <Arduino.h>
#include <SPI.h>
#include <EthernetUdp.h>
#include <Ethernet.h>
int count;
EthernetUDP Udp;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x99};
IPAddress groundStation1(10, 0, 0, 169);
IPAddress ip(10, 0, 0, 99);
int port = 42069;

void setup()
{
  Serial.begin(921600);
  pinMode(39, OUTPUT);
  // pinMode(8, INPUT);
  // pinMode(9, OUTPUT);
  // pinMode(7, OUTPUT);
  pinMode(35, INPUT);
  Ethernet.init(39);
  Ethernet.begin((uint8_t *)mac, ip, 37, 36, 38, 35);
  Udp.begin(port);

  Udp.beginPacket(groundStation1, port);
}

void loop()
{
  // Serial.println("before begin");
  // Serial.flush();
  while(1) {
    Udp.resetSendOffset();
    // Serial.println("before write");
    // Serial.flush();
    char tosend[] = "itimelckdatadatadatadata";
    // uint32_t beforeWrite = micros();
    Udp.write((unsigned char *) tosend, 24);
    // uint32_t beforeEnd = micros();
    // Serial.println("before end");
    // Serial.flush();
    Udp.endPacket();
    // uint32_t end = micros();

    // Serial.print("write: ");
    // Serial.print(beforeEnd - beforeWrite);
    // Serial.print(" end: ");
    // Serial.println(end - beforeEnd);
  }

  // Serial.println(count);
  // Serial.flush();
  // count += 1;

  // delay(1000);
}
