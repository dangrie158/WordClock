#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "NTP.h"

void NTP::setup() { mUdp.begin(NTP_LOCAL_PORT); }

void NTP::queryTime() {
  IPAddress timeServerIP;
  WiFi.hostByName(mServerName.c_str(), timeServerIP);
  Serial.print("Using NTP Server ");
  Serial.print(mServerName);
  Serial.print(" at: ");
  Serial.println(timeServerIP);
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(mPacketBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  mPacketBuffer[0] = 0b11100011; // LI, Version, Mode
  mPacketBuffer[1] = 0;          // Stratum, or type of clock
  mPacketBuffer[2] = 6;          // Polling Interval
  mPacketBuffer[3] = 0xEC;       // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  mPacketBuffer[12] = 49;
  mPacketBuffer[13] = 0x4E;
  mPacketBuffer[14] = 49;
  mPacketBuffer[15] = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  mUdp.beginPacket(timeServerIP, 123); // NTP requests are to port 123
  mUdp.write(mPacketBuffer, NTP_PACKET_SIZE);
  mUdp.endPacket();
}
void NTP::handlePackets() {
  int packetLength = mUdp.parsePacket();
  if (packetLength != 0) {
    Serial.print("NTP packet received, length: ");
    Serial.println(packetLength);
    mUdp.read(mPacketBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(mPacketBuffer[40], mPacketBuffer[41]);
    unsigned long lowWord = word(mPacketBuffer[42], mPacketBuffer[43]);

    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // subtract seventy years:
    unsigned long epoch = secsSince1900 - SEVENTY_YEARS;

    // print Unix time:
    Serial.print("Epoch: ");
    Serial.println(epoch);

    if (mTimeCallback != nullptr) {
      mTimeCallback(epoch);
    }
  }
}
