#include "lcd.h"
#include "stm32f1xx_hal.h"
#include "string.h"

// sendData ���������� ���� ������ �� ���� i2c �� ��� �������, ��� ��� �������� �� 4� ������� ����������
// pData - ����, �������� 0x25, ��� 2 (0010) ��� DB7-DB4 ��� DB3-DB0, � 5(0101) ��� ������� LED, E, RW, RS ��������������
// ����� ��������� ���������� � ������ ������ �� �������������� ���������� � datasheet � �������
void sendData(LCD1602 *scr , uint8_t *pData)
{
  *pData |= (1<<2);// ������������� ������������ ������ E � 1
  while(HAL_I2C_Master_Transmit(scr->hi2c,scr->DevAddress,pData,1,1000)!=HAL_OK)
  {}
  HAL_Delay(5); 
  *pData &=~(1<<2);// ������������� ������������ ������ E � 0
  while(HAL_I2C_Master_Transmit(scr->hi2c,scr->DevAddress,pData,1,1000)!=HAL_OK)
  {}
  HAL_Delay(5);
}
// initLCD ������� ��������� ������������� �������
void initLCD(LCD1602 *scr)
{
  uint8_t buf;
  buf = 0x30; // 0b00110000

  HAL_Delay(20);   //���� 15ms ���� ��������������� �������� ���������� (�������� ����� ��������� ����������)
  
  sendData(scr,&buf); //���������� ���� �����x buf
  sendData(scr,&buf); //��� ����
  sendData(scr,&buf); //� ������������ � ����������� �� ������������� �������
  
  buf = 0x20; // 0b00100000 ����� 4� ������� ����������
  sendData(scr,&buf);
  
  // ����� ��������� ����� ������� � ����� 0x00101000 (������ datasheet � �������)
  // ������� ������������ �� ��� ����, ��� ��� ������������� ���������
  buf = 0x20; // 0x20 � �������� ���� 0010 0000: 0010-������ �������� �������, 0000 - ������� rs,rw,e,led
  sendData(scr,&buf);
  buf = 0xC0; //1100 0000 N=1 ��� ������ F=1 (5*10), 0000 - ������� rs,rw,e,led
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
  
  //I/D - ��������� ����������� �������� ������� ����� ����� �������. (1 - �����, 0 - ������)
  //S - ����� ������� �������������� ������� ��������.
  buf =0;
  sendData(scr,&buf);
  buf = 0x30;
  sendData(scr,&buf);
  
  //�������� �������
  buf =0;
  sendData(scr,&buf);
  buf = 0xC8;
  sendData(scr,&buf);
  
}
//writeLCD ������� ������ s �� ����� scr
void writeLCD(LCD1602 *scr, uint8_t s)
{
    uint8_t command;
    command = ((s&0xf0)|0x09); //��������� ������� �������� � ������� ��� �������
    //command=(uint8_t)(0x18>>1);
    sendData(scr,&command);
    
    command = ((s&0x0f)<<4)|0x09;// ��������� ������ �������� � ������� ��� �������
    sendData(scr,&command);

}
void writeStringLCD(LCD1602 *scr,char *str)
{
  for (int i=0;i<strlen(str);i++)
  {
    writeLCD(scr,str[i]);
  }
}
// moveXY ����������� ������ � ������� X, Y
void moveXY(LCD1602 *scr,uint8_t x, uint8_t y)
{
	uint8_t command, adr;
	// �������� �� ������� �� x � y �� ������� ������������ ��������
	// � ������� ��� ������� �� 40 �������� � ������ �������� ��������
	if (y>1) y=1;
	if (x>39) x = 39;
	// ��������� ���������� x � y � ����� ������ DDRAM �������� ���������� � �������
	if (y==0)
		adr = x;
	if (y==1)
		adr = x+0x40;
	// ��� ��� ���������� ������������� ���������, ���������� ��� ������� ��� ����������� � ������ ������� DDRAM
	command = ((adr&0xf0)|0x80)&0xF8; //adr&0xf0 �������� ������� �������, |0x80 ������������� ������� ��� � 1(DB7=1 �������� ����������)&0xF8 ��������� 8(1000 - Led,E,Rw,Rs) � ������� �������
	sendData(scr,&command);

	command = (adr<<4)&0xF8;
	sendData(scr,&command);

}
