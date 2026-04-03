// Simulator/stim that sends set CANbus messages to the PT Motorsport Haltech CANbus shift light.
// Use an Arduino Uno and MCP2515 board.
// Many thanks to PT Motorsport.  
// SteamWolfAU


#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10); // CS pin D10

struct can_frame canMsg;

int rpm = 0;
int rpmStep = 100;
bool rpmUp = true;

void setup() {
Serial.begin(9600);
SPI.begin();

mcp2515.reset();
mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
mcp2515.setNormalMode();

Serial.println("CAN RPM Test Transmitter Started");

canMsg.can_id = 0x360; // Haltech RPM CAN ID
canMsg.can_dlc = 8; // DLC doesn't matter, only bytes 0 & 1 used
}

void loop() {

// Sweep RPM up and down
if (rpmUp) {
rpm += rpmStep;
if (rpm >= 8000) rpmUp = false;
} else {
rpm -= rpmStep;
if (rpm <= 1000) rpmUp = true;
}

// Encode RPM (Big Endian: MSB first)
canMsg.data[0] = (rpm >> 8) & 0xFF; // MSB
canMsg.data[1] = rpm & 0xFF; // LSB

// Fill remaining bytes with zeros
for (int i = 2; i < 8; i++) {
canMsg.data[i] = 0;
}

mcp2515.sendMessage(&canMsg);

Serial.print("Sent RPM: ");
Serial.println(rpm);

delay(50); // ~20 CAN messages per second
}
