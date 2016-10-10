#include "Arduino.h"
#include "LedControl.h"

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define DISPLAY_NUM_DEVICES 2

enum ClockWord {
  ES,
  IST,
  M_FUNF,
  M_ZEHN,
  M_ZWANZIG,
  M_DREIVIERTEL,
  M_VIERTEL,
  NACH,
  VOR,
  HALB,
  H_EINS,
  H_EIN,
  H_ZWEI,
  H_DREI,
  H_VIER,
  H_FUNF,
  H_SECHS,
  H_SIEBEN,
  H_ACHT,
  H_NEUN,
  H_ZEHN,
  H_ELF,
  H_ZWOLF,
  UHR
};

class Display {
private:
  LedControl *mLc;

  static uint8_t chipForPixel[10][11];
  static uint8_t rowForPixel[10][11];
  static uint8_t colForPixel[10][11];

  void doHourDisplay(int8_t hour, bool plural);

public:
  explicit Display(uint8_t data, uint8_t clock, uint8_t cs)
      : mLc(new LedControl(data, clock, cs, DISPLAY_NUM_DEVICES)) {}
  ~Display() { delete mLc; }
  void setup();
  void setLed(int x, int y, bool status);
  void showWord(ClockWord which);
  void clearDisplay();
  void setBrightness(int newBrightness);
  void showTime(int hours, int minutes, int seconds);
};

#endif //_DISPLAY_H_
