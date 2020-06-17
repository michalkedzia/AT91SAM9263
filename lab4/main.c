#include "project.h"
#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"


#define PERIOD 250000
#define DELAY 25
volatile int sec;
volatile int start;
volatile int stop;

void init_LEDS() {
	AT91C_BASE_PIOB->PIO_PER = AT91B_LED1;
	AT91C_BASE_PIOB->PIO_OER = AT91B_LED1;
	AT91C_BASE_PIOB->PIO_SODR = AT91B_LED1;
	AT91C_BASE_PIOC->PIO_PER = AT91B_LED2;
	AT91C_BASE_PIOC->PIO_OER = AT91B_LED2;
	AT91C_BASE_PIOC->PIO_SODR = AT91B_LED2;
}

void turn_on_LED1() {
	AT91C_BASE_PIOB->PIO_CODR = AT91B_LED1;
}

void turn_on_LED2() {
	AT91C_BASE_PIOC->PIO_CODR = AT91B_LED2;
}

void turn_off_LED1() {
	AT91C_BASE_PIOB->PIO_SODR = AT91B_LED1;
}

void turn_off_LED2() {
	AT91C_BASE_PIOC->PIO_SODR = AT91B_LED2;
}

int check_BP1() {
	return  (((AT91C_BASE_PIOC->PIO_PDSR) >> AT91B_BP1) & 1);
}

int check_BP2() {
	return  (((AT91C_BASE_PIOC->PIO_PDSR) >> AT91B_BP2) & 1);
}

volatile unsigned int checkPIT() {
	if (*AT91C_PITC_PISR & AT91C_PITC_PITS) {
		return 1;
	}
	return 0;
}

void PIT_interrupt_handler() {
	static volatile int  time = 0;
	static volatile int  flag = 0;

	if (checkPIT) {
		time++;
	}

	if (time == DELAY) {



		if (start) {
			sec = 1;
			start = 0;
			stop = 0;
		}
		else if (!stop) {

			if (sec == 9) {
				sec = 1;
			}
			else sec++;
		}




		if (flag == 0) {
			turn_on_LED1();
			flag = 1;
		}
		else {
			turn_off_LED1();
			flag = 0;
		}




		time = 0;
	}

}

void BTN_interrupt_handler() {

	volatile int b = (AT91C_BASE_PIOC->PIO_ISR & (AT91B_BP1 | AT91B_BP2));
	if (b & AT91B_BP1) {
		start = 1;
	}
	else if (b & AT91B_BP2) {
		stop = 1;
	}

}

void init_IRQ_PIT() {
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_SYS;
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (unsigned int)PIT_interrupt_handler;
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE | AT91C_AIC_PRIOR_HIGHEST;
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_SYS;
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_SYS;
	*AT91C_PITC_PIMR = AT91C_PITC_PITIEN | AT91C_PITC_PITEN | PERIOD;
}

void init_IRQ_BTN() {
	AT91C_BASE_PIOC->PIO_PER = AT91B_BP1 | AT91B_BP2;
	AT91C_BASE_PIOC->PIO_ODR = AT91B_BP1 | AT91B_BP2;
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_PIOCDE;
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_PIOCDE] = (unsigned int)BTN_interrupt_handler;
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_PIOCDE] = AT91C_AIC_SRCTYPE_EXT_HIGH_LEVEL | AT91C_AIC_PRIOR_HIGHEST;
	AT91C_BASE_AIC->AIC_ICCR = 1 << AT91C_ID_PIOCDE;
	AT91C_BASE_PIOC->PIO_IER = AT91B_BP1 | AT91B_BP2;
	AT91C_BASE_AIC->AIC_IECR = 1 << AT91C_ID_PIOCDE;
	AT91C_BASE_PIOC->PIO_PPUER = AT91B_BP1 | AT91B_BP2;
	*AT91C_PMC_PCER = (1 << AT91C_ID_PIOCDE);
}

int main(void) {

	init_LEDS();
	init_IRQ_PIT();
	init_IRQ_BTN();
	while (1) {}

	return 0;
}

