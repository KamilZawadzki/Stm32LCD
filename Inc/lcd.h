/* File led.hpp */
 #include "stm32f1xx_hal.h"
 #ifndef LED_H
 #define LED_H

  typedef struct
  {
    I2C_HandleTypeDef *hi2c;
    uint16_t DevAddress;
  }LCD1602;
  void sendData(LCD1602 *scr , uint8_t *pData);
  void initLCD(LCD1602 *scr);
  void writeLCD(LCD1602 *scr, uint8_t s);
  void writeStringLCD(LCD1602 *scr,char *str);
 #endif /* LED_H */
