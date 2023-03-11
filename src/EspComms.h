#pragma once

#include <Common.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include <Arduino.h>

#include <map>
#include <vector>

namespace Comms
{
  // const int port = 42069;
  // const IPAddress ip(10, 0, 0, IP_ADDRESS_END);
  // const IPAddress groundStation1(10, 0, 0, 69);
  // const IPAddress groundStation2(10, 0, 0, 70);

  struct Packet
  {
    uint8_t id;
    uint8_t len;
    uint8_t timestamp[4];
    uint8_t checksum[2];
    uint8_t data[256];
  };

  void init();

  typedef void (*commFunction)(Packet, uint8_t);

  /**
   * @brief Registers methods to be called when Comms receives a packet with a specific ID.
   *
   * @param id The ID of the packet associated with a specific command.
   * @param function a pointer to a method that takes in a Packet struct.
   */
  void registerCallback(uint8_t id, commFunction function);

  void processWaitingPackets();

  void packetAddFloat(Packet *packet, float value);
  void packetAddUint32(Packet *packet, uint32_t value);
  void packetAddUint16(Packet *packet, uint16_t value);
  void packetAddUint8(Packet *packet, uint8_t value);

  /**
   * @brief Interprets the packet data as a float.
   *
   * @param packet
   * @param index The index of the byte array at which the float starts (0, 4, 8).
   * @return float
   */
  float packetGetFloat(Packet *packet, uint8_t index);
  uint32_t packetGetUint32(Packet *packet, uint8_t index);
  uint32_t packetGetUint8(Packet *packet, uint8_t index);

  /**
   * @brief Sends packet data over ethernet and serial.
   *
   * @param packet The packet in which the data is stored.
   */
  void emitPacket(Packet *packet);

  /**
   * @brief Sends packet data over ethernet and serial towards a specific ip labeled socketNum
   *  Basically a refactoring of the earlier function.
   * @param packet 
   * @param socketNum 
   */
  void emitPacket(int socketNum, Packet *packet);

  bool verifyPacket(Packet *packet);

  uint16_t computePacketChecksum(Packet *packet);

  /**
   * @brief Sends the firmware version packet upon request
   *
   * @param _ unused
   */
  void sendFirmwareVersionPacket(Packet unused, uint8_t ip);
};
