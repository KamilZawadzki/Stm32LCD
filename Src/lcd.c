#include "lcd.h"
#include "stm32f1xx_hal.h"
#include "string.h"

void sendData(LCD1602 *scr , uint8_t *pData)
{
  *pData |= (1<<2);// устанавливаем стробирующий сигнал E в 1
  while(HAL_I2C_Master_Transmit(scr->hi2c,scr->DevAddress,pData,1,1000)!=HAL_OK)
  {}
  HAL_Delay(5); 
  *pData &=~(1<<2);// устанавливаем стробирующий сигнал E в 0
  while(HAL_I2C_Master_Transmit(scr->hi2c,scr->DevAddress,pData,1,1000)!=HAL_OK)
  {}
  HAL_Delay(5);
}
void initLCD(LCD1602 *scr)
{
  uint8_t buf;
  buf = 0x30; // 0b110000    

  HAL_Delay(20);   
  
  sendData(scr,&buf); //отправляем байт данныx buf
  sendData(scr,&buf); //отправляем байт данныx buf
  sendData(scr,&buf); //отправляем байт данныx buf
  
  buf = 0x20;
  sendData(scr,&buf);
  
  buf = 0x20;
  sendData(scr,&buf);
  buf = 0x80; // N=1 две строки F=1 (5*10) 
  sendData(scr,&buf);
  //display off
  buf =0;
  sendData(scr,&buf);
  buf = 0x80;
  sendData(scr,&buf);
  //display clear
  buf =0;
  sendData(scr,&buf);
  buf = 0x10;
  sendData(scr,&buf);
  
  //I/D - установка направления движения курсора после ввода символа. (1 - влево, 0 - вправо)
  //S - сдвиг курсора сопровождается сдвигом символов.
  buf =0;
  sendData(scr,&buf);
  buf = 0x30;
  sendData(scr,&buf);
  
  //включаем дисплей
  buf =0;
  sendData(scr,&buf);
  buf = 0xF8;
  sendData(scr,&buf);
  
}
void writeLCD(LCD1602 *scr, uint8_t s)
{
    uint8_t command;
    command = ((s&0xf0)|0x09);
    //command=(uint8_t)(0x18>>1);
    sendData(scr,&command);
    
    
    //HAL_Delay(1000);
    command = ((s&0x0f)<<4)|0x09;
    sendData(scr,&command);

}
void writeStringLCD(LCD1602 *scr,char *str)
{
  for (int i=0;i<strlen(str);i++)
  {
    writeLCD(scr,str[i]);
  }
   
    

}
