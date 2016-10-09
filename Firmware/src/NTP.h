#include "Arduino.h"

#ifndef _NTP_H_
#define _NTP_H_

#define NTP_LOCAL_PORT 2390
#define NTP_PACKET_SIZE 48
#define SEVENTY_YEARS 2208988800UL

class NTP {
private:
  WiFiUDP mUdp;
  void (*mTimeCallback)(unsigned long);
  byte mPacketBuffer[NTP_PACKET_SIZE];
  String mServerName;

public:
  void setup();
  void queryTime();
  inline void onNewTime(void (*cb)(unsigned long)) { mTimeCallback = cb; }
  void handlePackets();
  inline void setServer(String newServer) { mServerName = newServer; }
};

#endif //_NTP_H_
