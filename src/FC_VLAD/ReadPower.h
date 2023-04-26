#pragma once

#include <Arduino.h>
#include <INA233.h>
#include <WiFiComms.h>
#include "Radio.h"
#include "BlackBox.h"

//reads power stats from INA233 and sends to ground station
namespace Power
{
  void init();
  uint32_t task_readSendPower();
  void print();
}