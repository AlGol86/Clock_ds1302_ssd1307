//this example is intended for 8 MHz clock frequency
#include "iostm8s103f3.h"
#include "rtc.h"
#include "oled.h"


void delay_rtc(char del )
{
char i;
for(i=0;i<(del);i++)
{asm("nop");}
}
void rtc_set_time(void)
{
 send_rtc(CONTR,WHOLE, 0x00);  //WRITE-PROTECT: OFF
 send_rtc(TR_CH, WHOLE,0);    //TRICKLE CHARGER: OFF
 send_rtc(YEAR, 10,YEAR_10_TIME); 
 send_rtc(YEAR, 1, YEAR_1_TIME); 
 send_rtc(DAY, 1,MN_TH_ST);    //DAY OF WEEK
 send_rtc(MONTH, 10,MONTH_10_TIME); 
 send_rtc(MONTH, 1, MONTH_1_TIME); 
 send_rtc(DATE, 10,DATE_10_TIME); 
 send_rtc(DATE, 1, DATE_1_TIME); 
 send_rtc(HR, 10,HR_10_TIME); 
 send_rtc(HR, 1, HR_1_TIME); 
 send_rtc(MIN, 10,MIN_10_TIME); 
 send_rtc(MIN, 1, MIN_1_TIME); 
 send_rtc(SEC, WHOLE, 0);
}

void init_rtc(void)
{
  PC_CR2&=~MASK_clk_rst; // low speed (interrupt disabled)
  PC_CR1&=~MASK_clk_rst; // open drain /float in inp. direct
  PC_ODR&=~MASK_clk_rst;  //clk_pin, rst_pin  - 0 
  PC_DDR|=MASK_clk_rst; 
  PC_CR2&=~BIT_dat_rtc; //dat_pin  - low speed (interrupt disabled)
  PC_CR1&=~BIT_dat_rtc; //dat_pin - open drain /float in inp. direct
  PC_ODR|=BIT_dat_rtc;  //dat_pin  - 1 (SDA)
  PC_DDR|=BIT_dat_rtc; 
 }

void send_byte_rtc(char data)
{
for(char i=1;i<=8;i++)
{
PC_ODR&=((data&1)<<PIN_dat_rtc)|(~BIT_dat_rtc);
data>>=1;
PC_ODR|=BIT_clk_rtc; // clk=1
asm("nop");
PC_ODR|=BIT_dat_rtc; // sda=1
PC_ODR&=~BIT_clk_rtc;// clk=0
}
}

char  receive_byte_rtc(void)
{
char data=0;
for(char i=0;i<=7;i++)
{
asm("nop");
PC_ODR|=BIT_clk_rtc; // clk=1
data|=((PC_IDR&BIT_dat_rtc)>>PIN_dat_rtc)<<i;
asm("nop");
PC_ODR&=~BIT_clk_rtc;// clk=0
asm("nop");
}
return (data);
}

void send_rtc(char reg_adr, char part_of_data, char data)
{ 
  char data_0=0xFF;
  if(part_of_data!=WHOLE) data_0=receive_rtc(reg_adr, WHOLE);
  delay_rtc(3); //9*0.5us 
  PC_ODR|=BIT_rst_rtc; //rst=1(>4us)
  delay_rtc(3); //9*0.5us 
  reg_adr<<=1;
  reg_adr+=0x80;
  send_byte_rtc(reg_adr);
  asm("nop");
  switch(part_of_data)
  {
  case 1     :
    {send_byte_rtc((data_0&0xF0)|data);
    break;}
  case 10    :
    {send_byte_rtc((data<<4)|(data_0&0x0f));
    break;} 
  case WHOLE :
    {send_byte_rtc(data);
    break;} 
  }
  PC_ODR&=~BIT_rst_rtc; //rst=0
}

char receive_rtc(char reg_adr, char part_of_data)
{
  char receive_data=0xff;
  delay_rtc(3); //9*0.5us 
  PC_ODR|=BIT_rst_rtc; //rst=1(>4us)
  delay_rtc(3); //9*0.5us 
  
  reg_adr<<=1;
  reg_adr+=0x81;
  send_byte_rtc(reg_adr);
  switch(part_of_data)
  {
  case 1     :{receive_data=0x0F&receive_byte_rtc();break;}
  case 10    :{receive_data=(receive_byte_rtc()>>4);break;}
  case WHOLE :{receive_data=receive_byte_rtc();break;}
  }
  PC_ODR&=~BIT_rst_rtc; //rst=0
  return receive_data;
}

void print_time(void)
{ 
  oled_print_mega_digit(receive_rtc(HR, 10),14,1); //9*15
  oled_print_mega_digit(receive_rtc(HR, 1),27,1); //9*15

  oled_print_mega_digit(receive_rtc(MIN, 10),51,1); //9*15
  oled_print_mega_digit(receive_rtc(MIN, 1),64,1); //9*15

  oled_print_mega_digit(receive_rtc(SEC, 10),88,1); //9*15
  oled_print_mega_digit(receive_rtc(SEC, 1),101,1); //9*15
}

void print_time_giga(void)
{
  oled_print_giga_digit(receive_rtc(HR, 10),4,1); //9*15
  oled_print_giga_digit(receive_rtc(HR, 1),20,1); //9*15
  oled_print_giga_char(':',35,1);
  oled_print_giga_digit(receive_rtc(MIN, 10),50,1); //9*15
  oled_print_giga_digit(receive_rtc(MIN, 1),66,1); //9*15
  oled_print_giga_char(':',81,1);
  oled_print_giga_digit(receive_rtc(SEC, 10),96,1); //9*15
  oled_print_giga_digit(receive_rtc(SEC, 1),112,1); //9*15
}
