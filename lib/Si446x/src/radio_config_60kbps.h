
#ifndef RADIO_CONFIG_H_
#define RADIO_CONFIG_H_

#define RF_POWER_UP 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80
#define RF_GPIO_PIN_CFG 0x13, 0x41, 0x41, 0x21, 0x20, 0x67, 0x4B, 0x00
#define GLOBAL_2_0 0x11, 0x00, 0x04, 0x00, 0x52, 0x00, 0x18, 0x30
#define MODEM_2_0 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x02, 0x71, 0x00, 0x05, 0xC9, 0xC3, 0x80, 0x00, 0x00
#define MODEM_2_1 0x11, 0x20, 0x01, 0x0C, 0x46
#define MODEM_2_2 0x11, 0x20, 0x0C, 0x1C, 0x80, 0x00, 0xB0, 0x10, 0x0C, 0xE8, 0x00, 0x4E, 0x06, 0x8D, 0xB9, 0x00
#define MODEM_2_3 0x11, 0x20, 0x0A, 0x28, 0x00, 0x02, 0xC0, 0x08, 0x00, 0x12, 0xC6, 0xD4, 0x01, 0x5C
#define MODEM_2_4 0x11, 0x20, 0x0B, 0x39, 0x11, 0x11, 0x80, 0x1A, 0x20, 0x00, 0x00, 0x28, 0x0C, 0xA4, 0x23
#define MODEM_2_5 0x11, 0x20, 0x09, 0x45, 0x03, 0x00, 0x85, 0x01, 0x00, 0xFF, 0x06, 0x09, 0x10
#define MODEM_2_6 0x11, 0x20, 0x02, 0x50, 0x94, 0x0A
#define MODEM_2_7 0x11, 0x20, 0x02, 0x54, 0x03, 0x07
#define MODEM_2_8 0x11, 0x20, 0x05, 0x5B, 0x40, 0x04, 0x04, 0x78, 0x20
#define MODEM_CHFLT_2_0 0x11, 0x21, 0x0C, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00
#define MODEM_CHFLT_2_1 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x03, 0x15, 0xF0, 0x3F, 0x00, 0x7E, 0x64, 0x1B, 0xBA, 0x58, 0x0B
#define MODEM_CHFLT_2_2 0x11, 0x21, 0x0B, 0x18, 0xDD, 0xCE, 0xD6, 0xE6, 0xF6, 0x00, 0x03, 0x03, 0x15, 0xF0, 0x3F
#define PA_2_0 0x11, 0x22, 0x01, 0x03, 0x1D
#define FREQ_CONTROL_2_0 0x11, 0x40, 0x08, 0x00, 0x3B, 0x09, 0x00, 0x00, 0x44, 0x44, 0x20, 0xFE
#define RF_START_RX 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define RF_IRCAL 0x17, 0x56, 0x10, 0xCA, 0xF0
#define RF_IRCAL_1 0x17, 0x13, 0x10, 0xCA, 0xF0
#define INT_CTL_5_0 0x11, 0x01, 0x04, 0x00, 0x07, 0x18, 0x00, 0x00
#define FRR_CTL_5_0 0x11, 0x02, 0x03, 0x00, 0x0A, 0x09, 0x00
#define PREAMBLE_5_0 0x11, 0x10, 0x01, 0x04, 0x31
#define SYNC_5_0 0x11, 0x11, 0x04, 0x01, 0xB4, 0x2B, 0x00, 0x00
#define PKT_5_0 0x11, 0x12, 0x0A, 0x00, 0x04, 0x00, 0x30, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x2A, 0x01
#define PKT_5_1 0x11, 0x12, 0x07, 0x0E, 0x01, 0x06, 0xAA, 0x00, 0x80, 0x02, 0x2A
#define MODEM_5_0 0x11, 0x20, 0x0A, 0x03, 0x12, 0x4F, 0x80, 0x09, 0xC9, 0xC3, 0x80, 0x00, 0x08, 0x31
#define MODEM_5_1 0x11, 0x20, 0x0B, 0x1E, 0x10, 0x10, 0x00, 0xE8, 0x00, 0x53, 0x06, 0x24, 0xDD, 0x06, 0x2B
#define MODEM_5_2 0x11, 0x20, 0x02, 0x2A, 0x00, 0x00
#define MODEM_5_3 0x11, 0x20, 0x02, 0x31, 0xCA, 0xE0
#define MODEM_5_4 0x11, 0x20, 0x05, 0x39, 0x12, 0x12, 0x80, 0x1A, 0x40
#define MODEM_5_5 0x11, 0x20, 0x07, 0x47, 0xF9, 0x02, 0x00, 0x80, 0x06, 0x02, 0x18
#define MODEM_5_6 0x11, 0x20, 0x01, 0x50, 0x84
#define MODEM_5_7 0x11, 0x20, 0x01, 0x5D, 0x07
#define MODEM_CHFLT_5_0 0x11, 0x21, 0x0C, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11
#define MODEM_CHFLT_5_1 0x11, 0x21, 0x0C, 0x0C, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1
#define MODEM_CHFLT_5_2 0x11, 0x21, 0x0B, 0x18, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11, 0x0A, 0x04, 0x15, 0xFC, 0x03
#define FREQ_CONTROL_5_0 0x11, 0x40, 0x01, 0x01, 0x08

#define RADIO_CONFIGURATION_DATA_ARRAY { \
0x07, RF_POWER_UP, \
0x08, RF_GPIO_PIN_CFG, \
0x08, GLOBAL_2_0, \
0x10, MODEM_2_0, \
0x05, MODEM_2_1, \
0x10, MODEM_2_2, \
0x0E, MODEM_2_3, \
0x0F, MODEM_2_4, \
0x0D, MODEM_2_5, \
0x06, MODEM_2_6, \
0x06, MODEM_2_7, \
0x09, MODEM_2_8, \
0x10, MODEM_CHFLT_2_0, \
0x10, MODEM_CHFLT_2_1, \
0x0F, MODEM_CHFLT_2_2, \
0x05, PA_2_0, \
0x0C, FREQ_CONTROL_2_0, \
0x08, RF_START_RX, \
0x05, RF_IRCAL, \
0x05, RF_IRCAL_1, \
0x08, INT_CTL_5_0, \
0x07, FRR_CTL_5_0, \
0x05, PREAMBLE_5_0, \
0x08, SYNC_5_0, \
0x0E, PKT_5_0, \
0x0B, PKT_5_1, \
0x0E, MODEM_5_0, \
0x0F, MODEM_5_1, \
0x06, MODEM_5_2, \
0x06, MODEM_5_3, \
0x09, MODEM_5_4, \
0x0B, MODEM_5_5, \
0x05, MODEM_5_6, \
0x05, MODEM_5_7, \
0x10, MODEM_CHFLT_5_0, \
0x10, MODEM_CHFLT_5_1, \
0x0F, MODEM_CHFLT_5_2, \
0x05, FREQ_CONTROL_5_0, \
}

#endif
