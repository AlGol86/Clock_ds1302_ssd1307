#include "iostm8s103f3.h"
#include "iic.h"
#include "oled.h"
#include "rtc.h"

int main()
{
 init_rtc();
 //rtc_set_time();// uncomment for set real time (set time in rtc.h)
  
 init_ssd1306_2(0x3c);
 oled_blink1(0, 127, 20,1);// graphic start ection
 
 while(1)
 { 
   delay(30000);
   print_time_giga();
 }
}
