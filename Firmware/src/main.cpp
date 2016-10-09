

#include <DS1307RTC.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <WiFiClient.h>
#include <Wire.h>

#include "LedControl.h"

#include "ConfigServer.h"
#include "Display.h"
#include "NTP.h"

#define WIFI_SSID "WordClock"
#define WIFI_PASS "wordclock"

ConfigServer server(80);
Display display(13, 14, 12);
NTP ntp;

Ticker timeQueryTask;

wl_status_t mWiFiStatus;

void queryTime() { ntp.queryTime(); }

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) {
  case WIFI_EVENT_STAMODE_CONNECTED:
    Serial.println("Connected to WiFi");
    Serial.print("new IP:");
    Serial.println(WiFi.localIP());
    Serial.println("Disabling SoftAP");
    WiFi.softAPdisconnect();
    Serial.println("Attaching NTP Task");
    timeQueryTask.attach(3, queryTime);
    break;
  case WIFI_EVENT_STAMODE_DISCONNECTED:
    Serial.println("WLAN Connection lost");
    Serial.println("Reenabling SoftAP...");
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    Serial.println("Detaching NTP Task");
    timeQueryTask.detach();
    break;
  }
}

void epochToLocalTime(unsigned long epoch, int *hours, int *minutes,
                      int *seconds) {
  *seconds = epoch % 60;
  *minutes = (epoch % (60 * 60)) / 60;
  *hours = (epoch % (60 * 60 * 24)) / (60 * 60);

  // adjust the timezone
  *hours += server.getConfig()->timezone.toInt();
}

void setup() {
  WiFi.disconnect(true);
  Serial.begin(115200);

  server.setup();
  display.setup();
  ntp.setup();

  Config *config = server.getConfig();
  String station = config->station;
  String pass = config->passphrase;
  String server = config->server;

  ntp.setServer(server);

  WiFi.onEvent(WiFiEvent);

  if (station.length() > 0) {
    WiFi.begin(station.c_str(), pass.c_str());
  }

  // create the WiFi AP to configure the clock
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // set the NTP callback
  ntp.onNewTime([](unsigned long epoch) {
    int seconds;
    int minutes;
    int hours;
    epochToLocalTime(epoch, &hours, &minutes, &seconds);

    Serial.print("Time: ");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);

    Serial.println("Recalibrating Time");
    tmElements_t tm;
    tm.Hour = hours;
    tm.Minute = minutes;
    tm.Second = seconds;
    RTC.write(tm);
  });
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

void loop() {
  Config *config = server.getConfig();

  display.setLed(1, 2, true);

  server.handleClient();
  ntp.handlePackets();

  // sync brightness with config
  int brightness = config->brightness.toInt();
  display.setBrightness(brightness);

  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    // TODO: loading animation
  }
}
