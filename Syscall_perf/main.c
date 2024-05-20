#define MY_DEV_IRQ  5      // device uses IRQ 5 
#define MY_DEV_PRIO  2       // device uses interrupt priority 2 level privilege
#define MY_ISR_ARG  DEVICE_GET(my_device)
#define MY_IRQ_FLAGS 0       // IRQ flags 

#define CYCCNT 0xE0001004
#define CPICNT 0xE0001008
#define EXCCNT 0xE000100C
#define SLEEPCNT 0xE0001010
#define LSUCNT 0xE0001014
#define FOLDCNT 0xE0001018

#include <zephyr/kernel.h>
#include <stdio.h>
#include <zephyr/arch/arm/aarch32/cortex_m/cpu.h>
#include "thread_def.h"

void IRQ_Handler(void){
	printf("Hello from interruption handler\n");
	while(1){

	}
}

void launch_error(){
    volatile uint32_t *adr = (uint32_t *)0xFFFFFFFF;
    *adr = 0;
}

int main(void)
{

	printf("Main Thread started; %s\n", CONFIG_BOARD);

	volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000;    	//
	volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0;    	// Control Registers of DWT 
	volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC;		//


	volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)CYCCNT; 			// Cycle counter  
	volatile unsigned int *DWT_CPICNT   = (volatile unsigned int *)CPICNT; 			// Multi cycle counter
	volatile unsigned int *DWT_EXCCNT	= (volatile unsigned int *)EXCCNT; 			// Cycle spent in exception counter
	volatile unsigned int *DWT_SLEEPCNT	= (volatile unsigned int *)SLEEPCNT;		// Cycle spent sleeping counter
	volatile unsigned int *DWT_LSUCNT	= (volatile unsigned int *)LSUCNT;			// Cycle spent waiting for loads and stores to complete
	volatile unsigned int *DWT_FOLDCNT	= (volatile unsigned int *)FOLDCNT;			// Cycle saved by instructions which execute in zero cycles

	*DWT_CYCCNT 	= 0; 			// Reset the counters
	*DWT_CPICNT		= 0;
	*DWT_EXCCNT		= 0;
	*DWT_SLEEPCNT	= 0;
	*DWT_LSUCNT		= 0;
	*DWT_FOLDCNT	= 0;

	*DWT_LAR 		= 0xC5ACCE55; 	// unlock (CM7)
	*SCB_DEMCR 		|= 0x01000000;
	*DWT_CONTROL 	|= 0xFFFFFFFF; 	// enable the counter
	
	IRQ_CONNECT(MY_DEV_IRQ, MY_DEV_PRIO, IRQ_Handler,0,MY_IRQ_FLAGS);
	irq_enable(MY_DEV_IRQ);
	launch_error();

	k_thread_create(&supervisor_thread, supervisor_stack, THREAD_STACKSIZE,
			supervisor_thread_function, NULL, NULL, NULL,
			-1, K_INHERIT_PERMS, K_NO_WAIT);

	k_sleep(K_MSEC(1000));
	
	k_thread_create(&user_thread, user_stack, THREAD_STACKSIZE,
			user_thread_function, NULL, NULL, NULL,
			-1, K_USER | K_INHERIT_PERMS, K_NO_WAIT);
	return 0;
}
