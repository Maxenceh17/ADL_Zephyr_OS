

#define CYCCNT 0xE0001004
#define CPICNT 0xE0001008
#define EXCCNT 0xE000100C
#define SLEEPCNT 0xE0001010
#define LSUCNT 0xE0001014f
#define FOLDCNT 0xE0001018

#include <zephyr/kernel.h>
#include <stdio.h>
#include <zephyr/arch/arm/aarch32/cortex_m/cpu.h>
#include <zephyr/arch/arm/aarch32/arch.h>


void user_thread_function(void *p1, void *p2, void *p3)
{
	volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)CYCCNT; 		// Cycle counter  
	volatile unsigned int *DWT_CPICNT   = (volatile unsigned int *)CPICNT; 		// Multi cycle counter
	volatile unsigned int *DWT_EXCCNT	= (volatile unsigned int *)EXCCNT; 		// Cycle spent in exception counter
	volatile unsigned int *DWT_SLEEPCNT	= (volatile unsigned int *)SLEEPCNT;	// Cycle spent sleeping counter
	volatile unsigned int *DWT_LSUCNT	= (volatile unsigned int *)LSUCNT;		// Cycle spent waiting for loads and stores to complete
	volatile unsigned int *DWT_FOLDCNT	= (volatile unsigned int *)FOLDCNT;		// Cycle saved by instructions which execute in zero cycles
	
	k_tid_t thread;

	unsigned int instruction_counter 	= 0;
	unsigned int cycle_counter			= 0;
	
	
	printf("User thread started\n");
	
	while (1) {
		k_sleep(K_MSEC(2000));
		thread = 0;

		
		cycle_counter			= *DWT_CYCCNT;
		instruction_counter 	= *DWT_CYCCNT - *DWT_CPICNT - *DWT_EXCCNT - *DWT_SLEEPCNT - *DWT_LSUCNT + *DWT_FOLDCNT;
		
		launch_error();
		
		thread = k_current_get();
		
		instruction_counter		= (*DWT_CYCCNT - *DWT_CPICNT - *DWT_EXCCNT - *DWT_SLEEPCNT - *DWT_LSUCNT + *DWT_FOLDCNT)
										- instruction_counter;
		cycle_counter	 		= *DWT_CYCCNT - cycle_counter;

		//irq_disable(MY_DEV_IRQ);

		printf("User thread : (%p) 	Instruction Counter : %d	Cycle Counter : %d \n",thread,instruction_counter,cycle_counter);
		
	}
}
