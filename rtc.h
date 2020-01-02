#define YEAR_TIME  2020
#define MONTH_TIME 1
#define DATE_TIME  2
#define WEEK_TIME  4
#define HOURS_TIME 18
#define MINUTES    31

#define YEAR_10_TIME   ((YEAR_TIME%100)/10)
#define YEAR_1_TIME    (YEAR_TIME%10)
#define MN_TH_ST        WEEK_TIME
#define MONTH_10_TIME   (MONTH_TIME/10)
#define MONTH_1_TIME    (MONTH_TIME%10)
#define DATE_10_TIME    (DATE_TIME/10)
#define DATE_1_TIME     (DATE_TIME%10)
#define HR_10_TIME      (HOURS_TIME/10)
#define HR_1_TIME       (HOURS_TIME%10)
#define MIN_10_TIME     (MINUTES/10) 
#define MIN_1_TIME      (MINUTES%10) 

#define PIN_dat_rtc     4   //port D
#define PIN_clk_rtc     5   //port D
#define PIN_rst_rtc     6   //port D

#define BIT_dat_rtc    (1<<PIN_dat_rtc)
#define BIT_clk_rtc    (1<<PIN_clk_rtc)
#define BIT_rst_rtc    (1<<PIN_rst_rtc)
#define MASK_clk_rst   (BIT_clk_rtc|BIT_rst_rtc)

#define SEC   0
#define MIN   1
#define HR    2
#define DATE  3
#define MONTH 4
#define DAY   5
#define YEAR  6
#define CONTR 7
#define TR_CH 8
#define WHOLE 3

void delay_rtc(char del );
void rtc_set_time(void);
void init_rtc(void);
void send_byte_rtc(char data);
char receive_byte_rtc(void);
void send_rtc(char reg_adr, char part_of_data, char data);
char receive_rtc(char reg_adr, char part_of_data);
void print_time(void);
void print_time_giga(void);
