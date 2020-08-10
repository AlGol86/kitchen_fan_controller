#include "stm8s.h"

#define PORT_j1  GPIOD
#define pin_j1    6
#define bit_j1                       (1<<pin_j1)

#define PORT_j2  GPIOC
#define pin_j2    7
#define bit_j2                       (1<<pin_j2)

#define PORT_st_input  GPIOA
#define pin_st1_inp    1
#define bit_st1_inp                       (1<<pin_st1_inp)
#define pin_st2_inp    2
#define bit_st2_inp                       (1<<pin_st2_inp)
#define pin_st3_inp    3
#define bit_st3_inp                       (1<<pin_st3_inp)

#define PORT_but_ind   GPIOC
#define pin_ind_green  3
#define bit_ind_green                     (1<<pin_ind_green)
#define pin_ind_y_down 4
#define bit_ind_y_down                    (1<<pin_ind_y_down)
#define pin_ind_y_up   5
#define bit_ind_y_up                      (1<<pin_ind_y_up)
#define pin_ind_red    6
#define bit_ind_red                       (1<<pin_ind_red)

#define PORT_out       GPIOD
#define pin_led_green  4
#define bit_led_green                     (1<<pin_led_green)
#define pin_led_red    5
#define bit_led_red                       (1<<pin_led_red)
#define pin_step_down  3
#define bit_step_down                     (1<<pin_step_down)
#define pin_step_up    2
#define bit_step_up                       (1<<pin_step_up)

#define PORT_iic       GPIOB
#define pin_sda        4
#define bit_sda                           (1<<pin_sda)
#define pin_clk        5
#define bit_clk                           (1<<pin_clk)
#define WRITE 0
#define READ  1

#define address_iic_lm75 0x48
#define address_iic_lm75_1 (address_iic_lm75+1)
#define address_iic_lm75_2 (address_iic_lm75)
#define address_iic_lm75_3 (address_iic_lm75+4)
#define address_iic_lm75_4 (address_iic_lm75+5)



typedef enum {
	t1_error,
	t2_error,
	t3_error,
	t4_error,
	stage_error,
	stage_0,
	stage_1,
	stage_2,
	stage_3,
	step_up,
	step_down,
	off,
	green,
	red,
	red_toggle
} out_t ;

typedef enum {
norm,error_t1,error_t2,error_t3,error_t4} error_t ;

typedef struct 
{
	int t1[4];
	int t2[4];
	int t3[4];
	int t4[4];
	char tt[4];
	error_t t_err; 
	char actual_stage;
		
} input_data_t;

typedef struct 
{
	char hotplate_1  :1;
	char hotplate_2  :1;
	char hotplate_3  :1;
	char hotplate_4  :1;
	char stage;
		
} current_stage_t;

//iic
void sys_del_us(char del_us);
char start_iic (char adr, char read_write_bit);
char send_byte (char data_byte);
unsigned char receive_byte (char acknowledge);
void stop_iic (void);

void init_tim1(void);
@far @interrupt void TIM1Interrupt (void);

int get_temp(char addr);
out_t get_stage(void);
out_t calculated_stage(input_data_t temerature_data,current_stage_t* current_stage);
void output(out_t out);
void blink(out_t out);
void init_IO(void);
void sys_del_ms(unsigned int del_ms);
void action(out_t calc_stage, out_t actual_stage,current_stage_t* current_stage);


