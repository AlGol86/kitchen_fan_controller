/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
#include "stm8s.h"
#include "main.h"

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

@far @interrupt void TIM1Interrupt (void)
{
	extern input_data_t input_data;
	char i,j;
	int* int_pointer=&input_data.t1[0];
	TIM1->SR1&=~TIM1_SR1_UIF;//flag "0"
	for(j=0;j<4;j++)
		for(i=7;i>0;i--)
			*(int_pointer+i+j*8)=*(int_pointer+i-1+j*8);
	input_data.t1[0]= get_temp(address_iic_lm75_1);
  input_data.t2[0]= get_temp(address_iic_lm75_2);
	input_data.t3[0]= get_temp(address_iic_lm75_3);
	input_data.t4[0]= get_temp(address_iic_lm75_4);
	if ((input_data.t1[0]<10)||(input_data.t1[0]>950)) input_data.t_err=error_t1;
	 else if ((input_data.t2[0]<10)||(input_data.t2[0]>950)) input_data.t_err=error_t2;
	  else if ((input_data.t3[0]<10)||(input_data.t3[0]>950)) input_data.t_err=error_t3;
	   else if ((input_data.t4[0]<10)||(input_data.t4[0]>950)) input_data.t_err=error_t4;
	    else {
			input_data.t_err=norm;
			for(i=0;i<4;i++){
			if (((*(int_pointer+i*8))>(*(int_pointer+i*8+1)))&&\
			((*(int_pointer+i*8+1))>(*(int_pointer+i*8+2)))&&\
			((*(int_pointer+i*8+2))>(*(int_pointer+i*8+3)))&&\
                        ((*(int_pointer+i*8+3))>(*(int_pointer+i*8+4)))&&\
                        ((*(int_pointer+i*8+4))>(*(int_pointer+i*8+5)))&&\
                        ((*(int_pointer+i*8+5))>(*(int_pointer+i*8+6)))&&\
                        ((*(int_pointer+i*8+6))>(*(int_pointer+i*8+7)))) input_data.tt[i]=2;
			else 	if (((*(int_pointer+i*4))>(*(int_pointer+i*4+1)))||\
			((*(int_pointer+i*4+1))>(*(int_pointer+i*4+2)))||\
			((*(int_pointer+i*4+2))>(*(int_pointer+i*4+3)))) input_data.tt[i]=1;
			else input_data.tt[i]=0;
		  }
		}
  
	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, TIM1Interrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, NonHandledInterrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
