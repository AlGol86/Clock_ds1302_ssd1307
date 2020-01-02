 #include "iostm8s103f3.h"
#define oled_32_128_adr 0x3c
void init_ssd1306_2(char adr);
void delay(unsigned int del);
void sendCmd(char adr,char cmd);
void sendData(char adr,char* data, char count); //send [count] data byts 
void blankScreen(char adr,char n);
void set_cursor(char adr, char x,char y );
void ff_string(char adr,char n);
void send_repeat_data(char adr,char data_byte,char n);
void oled_blink1(char left, char right, char width, char count); //cursor right-left effect
void oled_print_small_digit(char d,char x,char y);  //3*5
void oled_print_middle_digit(char d,char x,char y); //6*10
void oled_print_mega_digit(char d,char x,char y);   //9*15
void oled_print_giga_digit(char d,char x,char y);   //12*20
void oled_print_giga_char(char c,char x,char y);    //12*20
void oled_print_terra_digit(char d,char x,char y);  //18*30
char oled_print_point(char n);
