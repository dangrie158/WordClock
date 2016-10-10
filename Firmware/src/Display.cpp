#include "Display.h"

/*
 * LED Numbers according to schematic. LED Numbers >64
 * are in the CHIP0 Matrix, the rest is CHIP1
 * 90   92   94   96   98  100   102  104  106  108  110
 * 89   91   93   95   97   99   101  103  105  107  109
 * 8    16   24   32   40   48   56   64   72   80   88
 * 7    15   23   31   39   47   55   63   71   79   87
 * 6    14   22   30   38   46   54   62   70   78   86
 * 5    13   21   29   37   45   53   61   69   77   85
 * 4    12   20   28   36   44   52   60   68   76   84
 * 3    11   19   27   35   43   51   59   67   75   83
 * 2    10   18   26   34   42   50   58   66   74   82
 * 1    9    17   25   33   41   49   57   65   73   81
 */

uint8_t Display::chipForPixel[10][11] = {
    /*
    c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 c10 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // row 0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // row 1
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 2
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 3
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 4
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 5
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 6
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 7
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // row 8
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}  // row 9
};

uint8_t Display::rowForPixel[10][11] = {
    /*
    c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 c10 */
    {1, 3, 5, 7, 1, 3, 5, 7, 1, 3, 5}, // row 0
    {0, 2, 4, 6, 0, 2, 4, 6, 0, 2, 4}, // row 1
    {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}, // row 2
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, // row 3
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, // row 4
    {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}, // row 5
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}, // row 6
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // row 7
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // row 8
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // row 9
};

uint8_t Display::colForPixel[10][11] = {
    /*
    c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 c10 */
    {4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6}, // row 0
    {4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6}, // row 1
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row2
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row 3
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row4
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row 5
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row 6
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row 7
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}, // row 8
    {1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3}  // row 9
};

void Display::setup() {
  // wake up the MAX72XX from power-saving mode
  mLc->shutdown(0, false);
  mLc->shutdown(1, false);
  // set a medium brightness for the Leds
  mLc->setIntensity(0, 3);
  mLc->setIntensity(1, 3);
}

void Display::setLed(int x, int y, bool status) {
  uint8_t col = Display::colForPixel[y][x];
  uint8_t row = Display::rowForPixel[y][x];
  uint8_t chip = Display::chipForPixel[y][x];
  mLc->setLed(chip, row, col, status);
}

void Display::showWord(ClockWord which) {
  switch (which) {
  case ES:
    setLed(0, 0, true);
    setLed(1, 0, true);
    break;
  case IST:
    setLed(3, 0, true);
    setLed(4, 0, true);
    setLed(5, 0, true);
    break;
  case M_FUNF:
    setLed(7, 0, true);
    setLed(8, 0, true);
    setLed(9, 0, true);
    setLed(10, 0, true);
    break;
  case M_ZEHN:
    setLed(0, 1, true);
    setLed(1, 1, true);
    setLed(2, 1, true);
    setLed(3, 1, true);
    break;
  case M_ZWANZIG:
    setLed(4, 1, true);
    setLed(5, 1, true);
    setLed(6, 1, true);
    setLed(7, 1, true);
    setLed(8, 1, true);
    setLed(9, 1, true);
    setLed(10, 1, true);
    break;
  case M_VIERTEL:
    setLed(4, 2, true);
    setLed(5, 2, true);
    setLed(6, 2, true);
    setLed(7, 2, true);
    setLed(8, 2, true);
    setLed(9, 2, true);
    setLed(10, 2, true);
    break;
  case M_DREIVIERTEL:
    setLed(0, 2, true);
    setLed(1, 2, true);
    setLed(2, 2, true);
    setLed(3, 2, true);
    setLed(4, 2, true);
    setLed(5, 2, true);
    setLed(6, 2, true);
    setLed(7, 2, true);
    setLed(8, 2, true);
    setLed(9, 2, true);
    setLed(10, 2, true);
    break;
  case NACH:
    setLed(2, 3, true);
    setLed(3, 3, true);
    setLed(4, 3, true);
    setLed(5, 3, true);
    break;
  case VOR:
    setLed(6, 3, true);
    setLed(7, 3, true);
    setLed(8, 3, true);
    break;
  case HALB:
    setLed(0, 4, true);
    setLed(1, 4, true);
    setLed(2, 4, true);
    setLed(3, 4, true);
    break;
  case H_EINS:
    setLed(2, 5, true);
    setLed(3, 5, true);
    setLed(4, 5, true);
    setLed(5, 5, true);
    break;
  case H_EIN:
    setLed(2, 5, true);
    setLed(3, 5, true);
    setLed(4, 5, true);
    break;
  case H_ZWEI:
    setLed(0, 5, true);
    setLed(1, 5, true);
    setLed(2, 5, true);
    setLed(3, 5, true);
    break;
  case H_DREI:
    setLed(1, 6, true);
    setLed(2, 6, true);
    setLed(3, 6, true);
    setLed(4, 6, true);
    break;
  case H_VIER:
    setLed(7, 7, true);
    setLed(8, 7, true);
    setLed(9, 7, true);
    setLed(10, 7, true);
    break;
  case H_FUNF:
    setLed(7, 6, true);
    setLed(8, 6, true);
    setLed(9, 6, true);
    setLed(10, 6, true);
    break;
  case H_SECHS:
    setLed(1, 9, true);
    setLed(2, 9, true);
    setLed(3, 9, true);
    setLed(4, 9, true);
    setLed(5, 9, true);
    break;
  case H_SIEBEN:
    setLed(5, 5, true);
    setLed(6, 5, true);
    setLed(7, 5, true);
    setLed(8, 5, true);
    setLed(9, 5, true);
    setLed(10, 5, true);
    break;
  case H_ACHT:
    setLed(1, 8, true);
    setLed(2, 8, true);
    setLed(3, 8, true);
    setLed(4, 8, true);
    break;
  case H_NEUN:
    setLed(3, 7, true);
    setLed(4, 7, true);
    setLed(5, 7, true);
    setLed(6, 7, true);
    break;
  case H_ZEHN:
    setLed(5, 8, true);
    setLed(6, 8, true);
    setLed(7, 8, true);
    setLed(8, 8, true);
    break;
  case H_ELF:
    setLed(0, 7, true);
    setLed(1, 7, true);
    setLed(2, 7, true);
    break;
  case H_ZWOLF:
    setLed(5, 4, true);
    setLed(6, 4, true);
    setLed(7, 4, true);
    setLed(8, 4, true);
    setLed(9, 4, true);
    break;
  case UHR:
    setLed(8, 9, true);
    setLed(9, 9, true);
    setLed(10, 9, true);
    break;
  }
}

void Display::clearDisplay() {
  mLc->clearDisplay(0);
  mLc->clearDisplay(1);
}

void Display::setBrightness(int newBrightness) {
  mLc->setIntensity(0, newBrightness);
  mLc->setIntensity(1, newBrightness);
}

void Display::showTime(int hours, int minutes, int seconds) {
  if (((minutes >= 57 && seconds >= 30) || minutes > 57) ||
      ((minutes < 3 && seconds < 30) || minutes < 2)) {
    // es ist ganz uhr
    this->showWord(ES);
    this->showWord(IST);
    if (minutes > 2) {
      // in previous hour
      this->doHourDisplay((hours + 1) % 24, false);
    } else {
      // current hour
      this->doHourDisplay(hours, false);
    }
    this->showWord(UHR);
  } else if ((minutes >= 52 && seconds >= 30) || minutes > 52) {
    // es ist fünf vor
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_FUNF);
    this->showWord(VOR);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 47 && seconds >= 30) || minutes > 47) {
    // es ist zehn vor
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_ZEHN);
    this->showWord(VOR);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 42 && seconds >= 30) || minutes > 42) {
    // es ist viertel vor
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_VIERTEL);
    this->showWord(VOR);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 37 && seconds >= 30) || minutes > 37) {
    // es ist zwanzig vor
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_ZWANZIG);
    this->showWord(VOR);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 32 && seconds >= 30) || minutes > 32) {
    // es ist fünf nach halb
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_FUNF);
    this->showWord(NACH);
    this->showWord(HALB);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 27 && seconds >= 30) || minutes > 27) {
    // es ist halb
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(HALB);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 22 && seconds >= 30) || minutes > 22) {
    // es ist fünf vor halb
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_FUNF);
    this->showWord(VOR);
    this->showWord(HALB);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes >= 17 && seconds >= 30) || minutes > 17) {
    // es ist zwanzig nach
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_ZWANZIG);
    this->showWord(NACH);
    this->doHourDisplay(hours, true);
  } else if ((minutes >= 12 && seconds >= 30) || minutes > 12) {
    // es ist viertel h+1
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_VIERTEL);
    this->doHourDisplay((hours + 1) % 24, true);
  } else if ((minutes == 7 && seconds >= 30) || minutes > 7) {
    // es ist zehn nach
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_ZEHN);
    this->showWord(NACH);
    this->doHourDisplay(hours, true);
  } else if ((minutes >= 2 && seconds >= 30) || minutes > 2) {
    // es ist fünf nach
    this->showWord(ES);
    this->showWord(IST);
    this->showWord(M_FUNF);
    this->showWord(NACH);
    this->doHourDisplay(hours, true);
  }
}

void Display::doHourDisplay(int8_t hour, bool plural) {
  if (hour == 0 || hour == 12) {
    this->showWord(H_ZWOLF);
  } else if (hour == 1 || hour == 13) {
    if (plural) {
      this->showWord(H_EINS);
    } else {
      this->showWord(H_EIN);
    }
  } else if (hour == 2 || hour == 14) {
    this->showWord(H_ZWEI);
  } else if (hour == 3 || hour == 15) {
    this->showWord(H_DREI);
  } else if (hour == 4 || hour == 16) {
    this->showWord(H_VIER);
  } else if (hour == 5 || hour == 17) {
    this->showWord(H_FUNF);
  } else if (hour == 6 || hour == 18) {
    this->showWord(H_SECHS);
  } else if (hour == 7 || hour == 19) {
    this->showWord(H_SIEBEN);
  } else if (hour == 8 || hour == 20) {
    this->showWord(H_ACHT);
  } else if (hour == 9 || hour == 21) {
    this->showWord(H_NEUN);
  } else if (hour == 10 || hour == 22) {
    this->showWord(H_ZEHN);
  } else if (hour == 11 || hour == 23) {
    this->showWord(H_ELF);
  }
}
