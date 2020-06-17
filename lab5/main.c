#include "project.h"
#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"

#define PERIOD 250000
#define DELAY 25
volatile int sec;
volatile int start;
volatile int stop;

#define PIN(P) (1 << (P))
#define A PIN(25)
#define B PIN(24)
#define C PIN(22)
#define D PIN(21)
#define E PIN(20)
#define F PIN(27)
#define G PIN(26)
#define DP PIN(23)
#define Q1 PIN(31)
#define Q2 PIN(28)
#define Q3 PIN(30)

const unsigned int SEG[10] = {
  A | B | C | D | E | F,
  B | C,
  A | B | G | E | D,
  A | B | C | D | G ,
  F | B | G | C ,
  A | F | G | C | D,
  A | F | G | E | D | C,
  A | B | C,
  A | B | C | D | E | F | G ,
  A | B | C | D | F | G ,
};

void init_LEDS() {

	AT91C_BASE_PIOB->PIO_PER = A | B | C | D | E | F | G | Q1 | Q2 | Q3 | DP;
	AT91C_BASE_PIOB->PIO_OER = A | B | C | D | E | F | G | Q1 | Q2 | Q3 | DP;
	AT91C_BASE_PIOB->PIO_CODR = A | B | C | D | E | F | G | Q1 | Q2 | DP;
	AT91C_BASE_PIOB->PIO_SODR = Q3;
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

void switch_segments(unsigned int digit) {
	AT91C_BASE_PIOB->PIO_SODR = Q2;
	AT91C_BASE_PIOB->PIO_CODR = A | B | C | D | E | F | G;
	AT91C_BASE_PIOB->PIO_SODR = SEG[digit];
	AT91C_BASE_PIOB->PIO_CODR = Q2;
}

void PIT_interrupt_handler() {
	static volatile int  time = 0;

	if (checkPIT) {
		time++;
	}

	if (time == DELAY) {

	    if (!stop) {

			if (!start) {
				if (sec == 9) {
					sec = 0;
					switch_segments(sec);
				}
				else {
					sec++;
					switch_segments(sec);
				}
			}
		}

		
		time = 0;
	}
}

void BTN_interrupt_handler() {

	volatile int b = (AT91C_BASE_PIOC->PIO_ISR & (AT91B_BP1 | AT91B_BP2));
	if (b & AT91B_BP1) {
		
		start = 1;
	
		switch_segments(0);
		sec = 0;
		start = 0;
		stop = 0;

		
		int val = *AT91C_PITC_PIVR;
	;
	}
	else if (b & AT91B_BP2) {
		stop = 1;
	}
}

void init_IRQ_PIT() {
	AT91C_BASE_AIC->AIC_IDCR = 1 << AT91C_ID_SYS;
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = PIT_interrupt_handler;
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
