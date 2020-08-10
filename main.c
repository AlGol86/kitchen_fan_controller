
#include "stm8s.h"
#include "main.h"

input_data_t input_data;
current_stage_t current_stage;

main()
{
init_IO();
sys_del_ms(10);
init_tim1();
sys_del_ms(3000);

while (1){
action(calculated_stage(input_data,&current_stage), get_stage(),&current_stage);
output(get_stage());
sys_del_ms(3000);
}

	
	
}

void action(out_t calc_stage, out_t actual_stage,current_stage_t* current_stage)
{
	char cnt_error=0;
	
if(actual_stage==stage_error)  {output(stage_error);cnt_error++;}
	 else if (calc_stage==t1_error) {output(t1_error);cnt_error++;}
	 else if (calc_stage==t2_error) {output(t2_error);cnt_error++;}
	 else if (calc_stage==t3_error) {output(t3_error);cnt_error++;}
	 else if (calc_stage==t4_error) {output(t4_error);cnt_error++;}
else{
	switch(actual_stage){
		case stage_0:{
			if (current_stage->stage==0) 
			 {
			 if (calc_stage!=stage_0) {
				output(step_up);
				output(green);
				current_stage->stage=1;
			  break;}
				else output(off);
			 }
			 else if (calc_stage!=stage_0) 
			                             output(red);
					else {current_stage->stage=0;output(off);}
		break;}
		
		case stage_1:{
		  if (current_stage->stage==1){ 
			 	if (calc_stage==stage_0) {
				    output(step_down);
						current_stage->stage=0;
						}
						else if ((calc_stage==stage_2)||(calc_stage==stage_3)) {
						     output(step_up);
								 current_stage->stage=2;
							 }
						}
       else 
			      output(red);						
	   break;}
	
	case stage_2:{
			if (current_stage->stage==2){ 
			 	if ((calc_stage==stage_0)||(calc_stage==stage_1)) {
					  output(step_down);
					  current_stage->stage=1;
					}
					else if (calc_stage!=stage_2) {
					  output(step_up);
					  current_stage->stage=3;
					}
				}
				else 
				     output(red);	
		
	   break;}
		 
	case stage_3:{
			if (current_stage->stage==3){ 
			  if (calc_stage!=stage_3) {
					  output(step_down);
					  current_stage->stage=2;
					}
				}
				else 
				     output(red);
				
				
				
		 break;}
  }
	
    }
if (cnt_error) output(red_toggle);
}

void init_tim1(void)
{
	TIM1->IER|=TIM1_IER_UIE;

  if(((PORT_j1->IDR&bit_j1)==0)&&((PORT_j2->IDR&bit_j2)==0))	{TIM1->PSCRH=0;TIM1->PSCRL=120;} //t=3s
  if(((PORT_j1->IDR&bit_j1)==0)&&(PORT_j2->IDR&bit_j2))	  {TIM1->PSCRH=0;TIM1->PSCRL=190;} //t=5s
	if((PORT_j1->IDR&bit_j1)&&((PORT_j2->IDR&bit_j2)==0))	  {TIM1->PSCRH=1;TIM1->PSCRL=20;} //t=7s
	if((PORT_j1->IDR&bit_j1)&&(PORT_j2->IDR&bit_j2))	      {TIM1->PSCRH=1;TIM1->PSCRL=90;} //t=9s
	TIM1->ARRH=200; 
  TIM1->ARRL=10;
	TIM1->CNTRH=0;
	TIM1->CNTRL=0;
	rim();
	TIM1->CR1|=TIM1_CR1_CEN;
}


int get_temp(char addr)
{
int t=0,tt=0;
start_iic (addr, READ);
t=receive_byte (1);
tt=receive_byte (0);
stop_iic ();
t<<=3;
tt>>=5;
t+=tt;
return (t);
}

out_t calculated_stage(input_data_t temerature_data,current_stage_t* current_stage)
{
	int t_summa=0;
	char t1,t2,t3,t4;
	char tt1,tt2,tt3,tt4;
	char aux_level=0;
	t1=temerature_data.t1[0]>>3;
	t2=temerature_data.t2[0]>>3;
	t3=temerature_data.t3[0]>>3;
	t4=temerature_data.t4[0]>>3;
	tt1=temerature_data.tt[0];
	tt2=temerature_data.tt[1];
	tt3=temerature_data.tt[2];
	tt4=temerature_data.tt[3];
	


	if (temerature_data.t_err!=norm) 
	if      (temerature_data.t_err==t1_error) return (t1_error);
	else if (temerature_data.t_err==t2_error) return (t2_error);
	else if (temerature_data.t_err==t3_error) return (t3_error);
	else if (temerature_data.t_err==t4_error) return (t4_error);
	
	
		if (t1>60) aux_level++;
		if (t2>60) aux_level++;
		if (t3>60) aux_level++;
		if (t4>60) aux_level++;
		if ((tt1==1)&&(current_stage->hotplate_1)&&(t1<=60))aux_level++;
		if ((tt2==1)&&(current_stage->hotplate_2)&&(t2<=60))aux_level++;
		if ((tt3==1)&&(current_stage->hotplate_3)&&(t3<=60))aux_level++;
		if ((tt4==1)&&(current_stage->hotplate_4)&&(t4<=60))aux_level++;
	        if ((tt1==2)&&(t1<=60)) {aux_level++;current_stage->hotplate_1=1;}
		if ((tt2==2)&&(t2<=60)) {aux_level++;current_stage->hotplate_2=1;}
		if ((tt3==2)&&(t3<=60)) {aux_level++;current_stage->hotplate_3=1;}
		if ((tt4==2)&&(t4<=60)) {aux_level++;current_stage->hotplate_4=1;}
		if (tt1==0) {current_stage->hotplate_1=0;}
		if (tt2==0) {current_stage->hotplate_2=0;}
		if (tt3==0) {current_stage->hotplate_3=0;}
		if (tt4==0) {current_stage->hotplate_4=0;}
		if (aux_level==0) return (stage_0);
		 else if (aux_level==1) return (stage_1);
		  else if (aux_level==2) return (stage_2);
			 else                   return (stage_3);
	 
}

out_t get_stage(void)
{
if ((PORT_st_input->IDR&(bit_st1_inp|bit_st2_inp|bit_st3_inp))==0)	return (stage_0);
 else if ((PORT_st_input->IDR&(bit_st1_inp|bit_st2_inp|bit_st3_inp))==bit_st1_inp) return (stage_1);
  else if ((PORT_st_input->IDR&(bit_st1_inp|bit_st2_inp|bit_st3_inp))==bit_st2_inp) return (stage_2);
	 else if ((PORT_st_input->IDR&(bit_st1_inp|bit_st2_inp|bit_st3_inp))==bit_st3_inp) return (stage_3);
	  else return (stage_error);
}

void output(out_t out)
{
 int i;
 char j;
 unsigned int step_del=300;
 switch (out){
	 
  case off:     {PORT_but_ind->ODR&=~(bit_ind_green|bit_ind_y_down|bit_ind_y_up|bit_ind_red);break;}
	case green:   {PORT_but_ind->ODR|=bit_ind_green;break;}
	case red:     {PORT_but_ind->ODR|=bit_ind_red;break;}
	case red_toggle: {
	PORT_but_ind->ODR^=bit_ind_red;
	sys_del_ms(500);
	PORT_but_ind->ODR^=bit_ind_red;
	break;}
	case t1_error:
	{blink(t1_error);break;}
	case t2_error:
	{blink(t2_error);break;}
	case t3_error:
	{blink(t3_error);break;}
	case t4_error:
	{blink(t4_error);break;}
	case stage_error:
	{blink(stage_error);break;}
	case stage_1:{blink(stage_1);break;}
	case stage_2:{blink(stage_2);break;}
	case stage_3:{blink(stage_3);break;} 
	
	case step_down:{
	 blink(step_down);
	 PORT_out ->ODR&=~bit_step_down;
	 sys_del_ms(step_del);
	 PORT_out ->ODR|=bit_step_down; 
	 break;}	
	 
	case step_up:{
	 blink(step_up);
	 PORT_out ->ODR&=~bit_step_up;
	 sys_del_ms(step_del);
	 PORT_out ->ODR|=bit_step_up; 
	 break;}	
}
}

void blink(out_t out)
{ 
 char i;
 unsigned int blink_del=200;
 switch (out){
  case t1_error:{
	  PORT_out ->ODR|=bit_led_red;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_red;
	  break;}
	case t2_error:{
		for(i=0;i<2;i++){
		sys_del_ms(blink_del);
		PORT_out ->ODR|=bit_led_red;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_red;
	  }break;}
	case t3_error:{
		for(i=0;i<3;i++){
		sys_del_ms(blink_del);
		PORT_out ->ODR|=bit_led_red;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_red;
	  }break;}
	case t4_error:{
		for(i=0;i<4;i++){
		sys_del_ms(blink_del);
		PORT_out ->ODR|=bit_led_red;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_red;
	  }break;}	
	case stage_error:{
		for(i=0;i<200;i++){
		sys_del_ms(50);
		PORT_out ->ODR|=bit_led_red;
		sys_del_ms(30);
		PORT_out ->ODR&=~bit_led_red;
	  }break;}	
	case stage_0:{break;}		
	case stage_1:{
		PORT_out ->ODR|=bit_led_green;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_green;
	  break;}	
	case stage_2:{
		for(i=0;i<2;i++){
		sys_del_ms(blink_del);
		PORT_out ->ODR|=bit_led_green;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_green;
	  }break;}	
	case stage_3:{
		for(i=0;i<3;i++){
		sys_del_ms(blink_del);
		PORT_out ->ODR|=bit_led_green;
		sys_del_ms(blink_del);
		PORT_out ->ODR&=~bit_led_green;
	  }break;}	
	case step_down:{
		for(i=0;i<10;i++){
		sys_del_ms(blink_del);
		PORT_but_ind ->ODR|=bit_ind_y_down;
		sys_del_ms(blink_del);
		PORT_but_ind ->ODR&=~bit_ind_y_down;
	  }break;}
	case step_up:{
		for(i=0;i<10;i++){
		sys_del_ms(blink_del);
		PORT_but_ind ->ODR|=bit_ind_y_up;
		sys_del_ms(blink_del);
		PORT_but_ind ->ODR&=~bit_ind_y_up;
	  }break;}
}}

void init_IO(void)
{
	//servis_leds & step outs
	PORT_out ->ODR|=bit_step_down|bit_step_up;
  PORT_out ->CR2&=~(bit_led_green|bit_led_red|bit_step_down|bit_step_up); 
	PORT_out ->CR1|=bit_led_green|bit_led_red;
	PORT_out ->DDR|=bit_led_red|bit_led_green|bit_step_down|bit_step_up; 
	PORT_out ->CR1&=~(bit_step_down|bit_step_up);
	
	//button leds
	PORT_but_ind->CR2&=~(bit_ind_green|bit_ind_y_down|bit_ind_y_up|bit_ind_red); 
	PORT_but_ind->CR1|=(bit_ind_green|bit_ind_y_down|bit_ind_y_up|bit_ind_red); 
	PORT_but_ind->DDR|=(bit_ind_green|bit_ind_y_down|bit_ind_y_up|bit_ind_red);
	
	//step inputs
	PORT_st_input->CR2&=~(bit_st1_inp|bit_st2_inp|bit_st3_inp);
	PORT_st_input->CR1&=~(bit_st1_inp|bit_st2_inp|bit_st3_inp);
	PORT_st_input->DDR&=~(bit_st1_inp|bit_st2_inp|bit_st3_inp);
	
	//jumpers
	PORT_j1->CR2&=~bit_j1;
	PORT_j1->CR1&=~bit_j1;
	PORT_j1->DDR&=~bit_j1;
	PORT_j2->CR2&=~bit_j2;
	PORT_j2->CR1&=~bit_j2;
	PORT_j2->DDR&=~bit_j2;
	
	
}

void sys_del_ms(unsigned int del_ms)	
{
unsigned long int i;
unsigned long int del_us;
del_us=30*(unsigned long int)del_ms;
for(i=0;i<del_us;i++)
{nop();}
}
