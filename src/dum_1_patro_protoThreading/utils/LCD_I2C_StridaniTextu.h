#ifndef LCD_I2C_STRIDANI_TEXTU_H
#define LCD_I2C_STRIDANI_TEXTU_H

#include <LiquidCrystal_I2C.h>
#include "StridaniAkci.h"

class LCD_I2C_StridaniTextu : public StridaniAkci<LCD_I2C_StridaniTextu> {
private:
  const char *const _text1;
  const uint8_t _text1X;
  const char *const _text2;
  const uint8_t _text2X;
  LiquidCrystal_I2C *const _lcd;
  const uint8_t _radekNaLcd;

  void zobrazitText1() {
    _lcd->setCursor(_text1X, _radekNaLcd);
    _lcd->print(_text1);
  }

  void zobrazitText2() {
    _lcd->setCursor(_text2X, _radekNaLcd);
    _lcd->print(_text2);
  }

public:
  LCD_I2C_StridaniTextu(const char *const text1,
                        const uint8_t text1X,
                        const char *const text2,
                        const uint8_t text2X,
                        LiquidCrystal_I2C *const lcd,
                        const uint8_t radekNaLcd,
                        bool zobrazitOkamzite)
    : StridaniAkci(this, &LCD_I2C_StridaniTextu::zobrazitText1, &LCD_I2C_StridaniTextu::zobrazitText2, zobrazitOkamzite),
      _text1(text1),
      _text1X(text1X),
      _text2(text2),
      _text2X(text2X),
      _lcd(lcd),
      _radekNaLcd(radekNaLcd) {
    
  }
};

#endif
