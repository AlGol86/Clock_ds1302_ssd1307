#include "iostm8s103f3.h"
#include "iic.h"
#include "oled.h"

void delay(unsigned int del)
{
  for(int j=0;j<=del;j++){}
}

void sendCmd(char adr,char cmd)
{start_iic(adr,WRITE);
send_byte (0x80);
send_byte (cmd);
stop_iic();
}

void sendData(char adr,char* data, char count)
{start_iic(adr,WRITE);
send_byte (0x40);
for(int j=0;j<count;j++)
{
send_byte (*(data+j));
}
stop_iic();
}

void init_ssd1306_2(char adr)
{
   delay(10000);
   sendCmd(adr,0xAE); //display off
   sendCmd(adr,0x20); //Set Memory Addressing Mode   
   sendCmd(adr,0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid
   sendCmd(adr,0xC0); //Set COM Output Scan Direction    (0xC0)-- Mirror vertically
   sendCmd(adr,0xA0); //--set segment re-map 0 to 127 - CHECK (0xA0)--- Mirror horizontally
   sendCmd(adr,0xB0); //Set Page Start Address for Page Addressing Mode,0-7
   sendCmd(adr,0x00); //---set low column address
   sendCmd(adr,0x10); //---set high column address
   sendCmd(adr,0x40); //--set start line address - CHECK
   sendCmd(adr,0x81); //--set contrast control register - CHECK
   sendCmd(adr,0x7F); //--set contrast volume
   sendCmd(adr,0xA6); //--set normal color (0xA7)--set inverse color
   sendCmd(adr,0xA8); //--set multiplex ratio(1 to 64) - CHECK
   sendCmd(adr,0x3F); //
   sendCmd(adr,0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
   sendCmd(adr,0xD3); //-set display offset - CHECK
   sendCmd(adr,0x00); //-not offset
   sendCmd(adr,0xD5); //--set display clock divide ratio/oscillator frequency
   sendCmd(adr,0xF0); //--set divide ratio
   sendCmd(adr,0xD9); //--set pre-charge period
   sendCmd(adr,0x22); //
   sendCmd(adr,0xDA); //--set com pins hardware configuration - CHECK
   sendCmd(adr,0x02); //12/02
   sendCmd(adr,0xDB); //--set vcomh
   sendCmd(adr,0x20); //0x20,0.77xVcc
   sendCmd(adr,0x8D); //--set DC-DC enable
   sendCmd(adr,0x14); //
   sendCmd(adr,0xAF); //--turn on SSD1306 panel
   for(int j=0;j<=3;j++) blankScreen(0x3c,128);
}

void blankScreen(char adr,char n)
{
send_repeat_data(adr,0x00,n);
}

void ff_string(char adr,char n)
{
send_repeat_data(adr,0xff,n);
}

void send_repeat_data(char adr,char data_byte,char n)
{
start_iic(adr,WRITE);
send_byte (0x40);
for(int j=1;j<=n;j++) send_byte (data_byte);
stop_iic();
}

void set_cursor(char adr, char x,char y )
{   sendCmd(adr,0x0f&x); 
    sendCmd(adr,0x10|(0x0f&(x>>4)));
    sendCmd(adr,0xb0|y); 
}

void oled_blink1(char left, char right, char width,char count)
 {  char i=left;
    char sig=0; 
    const char y=0;
    count++;
     while(count)
     {
      set_cursor(0x3c, i,y );
      ff_string(0x3c,width);
      set_cursor(0x3c, i+width-1,y );
      blankScreen(0x3c,1);
      set_cursor(0x3c, i,y );
      blankScreen(0x3c,1);
      if(i==right+1-width) sig=1;
      if(i==left)   {sig=0;count--;}
      if (sig==0) i++; else i--;
     } 
     set_cursor(0x3c, left,y );
     blankScreen(0x3c,width); 
 }

void oled_print_small_digit(char d,char x,char y)
{
  char dig0[3]={0x1f,0x11,0x1f};
 set_cursor(0x3c,x,y );
  switch(d)
  {
  case 0:{sendData(0x3c,dig0,3);break;}
  }
}

void oled_print_middle_digit(char d,char x,char y)
{
  const char c=6;
  char dig0[c+c]={0xff,0xff,0x03,0x03,0xff,0xff,0x03,0x03,0x03,0x03,0x03,0x03};
  set_cursor(0x3c,x,y );
  switch(d)
  {
  case 0:
    {sendData(0x3c,dig0,c);set_cursor(0x3c,x,y+1);sendData(0x3c,dig0+c,c);break;}
  } 
  }

void oled_print_mega_digit(char d,char x,char y)
{
  const char c=3;
  const char const_dig[10][6]={{0xff,0x07,0xff,0x7f,0x70,0x7f}, //0
                                 {0,0,0xff,0,0,0x7f},             //1
                                 {0xc7,0xc7,0xff,0x7f,0x71,0x71}, //2
                                 {0xc7,0xc7,0xff,0x71,0x71,0x7f}, //3            
                                 {0xff,0xc0,0xff,0x01,0x01,0x7f}, //4
                                 {0xff,0xc7,0xc7,0x71,0x71,0x7f}, //5
                                 {0xff,0xc7,0xc7,0x7f,0x71,0x7f}, //6
                                 {0x07,0x07,0xff,0x00,0x00,0x7f}, //7
                                 {0xff,0xc7,0xff,0x7f,0x71,0x7f}, //8
                                 {0xff,0xc7,0xff,0x71,0x71,0x7f}};//9
  char dig[6];
  for (char i=0;i<6;i++) dig[i]=const_dig[d][i]; 
  
  set_cursor(oled_32_128_adr,x,y );
  for (char i=0;i<3;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  set_cursor(oled_32_128_adr,x,y+1);
  for (char i=3;i<6;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
}

void oled_print_giga_digit(char d,char x,char y)
{
  const char c=4;
  const char const_dig[10][9]={{0xff,0x0f,0xff,0xff,0x00,0xff,0x0f,0x0f,0x0f},  //0
                               {0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0x0f},  //1
                               {0x0f,0x0f,0xff,0xff,0x0f,0x0f,0x0f,0x0f,0x0f},  //2
                               {0x0f,0x0f,0xff,0x0f,0x0f,0xff,0x0f,0x0f,0x0f},  //3
                               {0xff,0x00,0xff,0x0f,0x0f,0xff,0x00,0x00,0x0f},  //4
                               {0xff,0x0f,0x0f,0x0f,0x0f,0xff,0x0f,0x0f,0x0f},  //5
                               {0xff,0x0f,0x0f,0xff,0x0f,0xff,0x0f,0x0f,0x0f},  //6
                               {0x0f,0x0f,0xff,0x00,0x00,0xff,0x00,0x00,0x0f},  //7
                               {0xff,0x0f,0xff,0xff,0x0f,0xff,0x0f,0x0f,0x0f},  //8
                               {0xff,0x0f,0xff,0x0f,0x0f,0xff,0x0f,0x0f,0x0f}}; //9
  char dig[9];
  for (char i=0;i<10;i++) dig[i]=const_dig[d][i]; 
  
  set_cursor(oled_32_128_adr,x,y );
  for (char i=0;i<3;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  set_cursor(oled_32_128_adr,x,y+1);
  for (char i=3;i<6;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  set_cursor(oled_32_128_adr,x,y+2);
  for (char i=6;i<9;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  }

void oled_print_giga_char(char c,char x,char y)
{
  const char cc=3;
  const char const_ch[1][12]={{0x00,0x38,0x38,0x00, 0x00,0xc0,0xc0,0x00, 0x00,0x01,0x01,0x00}  //':'
                              }; 
  char ch[12];
  switch(c)
  {
  case ':':{c=0; break;}
  }
  
  
  for (char i=0;i<13;i++) ch[i]=const_ch[c][i]; 
  
  set_cursor(oled_32_128_adr,x,y );
  for (char i=0;i<4;i++) send_repeat_data(oled_32_128_adr,ch[i],cc);
  set_cursor(oled_32_128_adr,x,y+1);
  for (char i=4;i<8;i++) send_repeat_data(oled_32_128_adr,ch[i],cc);
  set_cursor(oled_32_128_adr,x,y+2);
  for (char i=8;i<12;i++) send_repeat_data(oled_32_128_adr,ch[i],cc);
  }

void oled_print_terra_digit(char d,char x,char y)
{
  const char c=6;
  const char const_dig[10][12]={{0xff,0x3f,0xff,0xff,0x00,0xff,0xff,0x00,0xff,0x3f,0x3f,0x3f},   //0
                                {0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0x3f},   //1  
                                {0x3f,0x3f,0xff,0xf0,0xf0,0xff,0xff,0x03,0x03,0x3f,0x3f,0x3f},   //2   
                                {0x3f,0x3f,0xff,0xf0,0xf0,0xff,0x03,0x03,0xff,0x3f,0x3f,0x3f},   //3 
                                {0xff,0x00,0xff,0xff,0xf0,0xff,0x03,0x03,0xff,0x00,0x00,0x3f},   //4 
                                {0xff,0x3f,0x3f,0xff,0xf0,0xf0,0x03,0x03,0xff,0x3f,0x3f,0x3f},   //5 
                                {0xff,0x3f,0x3f,0xff,0xf0,0xf0,0xff,0x03,0xff,0x3f,0x3f,0x3f},   //6 
                                {0x3f,0x3f,0xff,0x00,0x00,0xff,0x00,0x00,0xff,0x00,0x00,0x3f},   //7
                                {0xff,0x3f,0xff,0xff,0xf0,0xff,0xff,0x03,0xff,0x3f,0x3f,0x3f},   //8 
                                {0xff,0x3f,0xff,0xff,0xf0,0xff,0x03,0x03,0xff,0x3f,0x3f,0x3f}};  //9 
  char dig[12];
  for (char i=0;i<12;i++) dig[i]=const_dig[d][i]; 
  
  set_cursor(oled_32_128_adr,x,y );
  for (char i=0;i<3;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  set_cursor(oled_32_128_adr,x,y+1);
  for (char i=3;i<6;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  set_cursor(oled_32_128_adr,x,y+2);
  for (char i=6;i<9;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  set_cursor(oled_32_128_adr,x,y+3);
  for (char i=9;i<12;i++) send_repeat_data(oled_32_128_adr,dig[i],c);
  }
  
char oled_print_point(char n)
{
  char scale=(n/32)+1;
  char y;
  n/=scale;
  y=3-(n/8);
  sendCmd(oled_32_128_adr,0xb0|y); 
  send_repeat_data(oled_32_128_adr,1<<(7-n%8),1);
  return scale;
}


