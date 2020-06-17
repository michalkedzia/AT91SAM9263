#include "project.h"
#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"

#define BP1 5
#define BP2 4
#define PERIOD 250000
#define DELAY 25

void init_LEDS() {
	AT91C_BASE_PIOB->PIO_PER = AT91C_PIO_PB8;
	AT91C_BASE_PIOB->PIO_OER = AT91C_PIO_PB8;
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8;

	AT91C_BASE_PIOC->PIO_PER = AT91C_PIO_PC29;
	AT91C_BASE_PIOC->PIO_OER = AT91C_PIO_PC29;
	AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29;
}

void init_BPS() {
	*AT91C_PMC_PCER = (1 << AT91C_ID_PIOCDE);
	AT91C_BASE_PIOC->PIO_PER = AT91C_PIO_PC5 | AT91C_PIO_PC4;
	AT91C_BASE_PIOC->PIO_ODR = AT91C_PIO_PC5 | AT91C_PIO_PC4;
	AT91C_BASE_PIOC->PIO_PPUER = AT91C_PIO_PC5 | AT91C_PIO_PC4;
}


void turn_on_LED1() {
	AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB8;
}

void turn_on_LED2() {
	AT91C_BASE_PIOC->PIO_CODR = AT91C_PIO_PC29;
}

void turn_off_LED1() {
	AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB8;
}

void turn_off_LED2() {
	AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC29;
}

int check_BP1() {
	return  (((AT91C_BASE_PIOC->PIO_PDSR) >> BP1) & 1);
}

int check_BP2() {
	return  (((AT91C_BASE_PIOC->PIO_PDSR) >> BP2) & 1);
}

volatile unsigned int checkPIT() {
	if (*AT91C_PITC_PISR & AT91C_PITC_PITS) {
		volatile unsigned int val = AT91C_BASE_PITC->PITC_PIVR;
		val = *AT91C_PITC_PIVR;
		return 1;
	}
	return 0;
}

int main(void) {

	*AT91C_PITC_PIMR = AT91C_PITC_PITEN | PERIOD;
	init_LEDS();
	init_BPS();
	turn_off_LED1();

	volatile int seconds = 0, btn1 = 0, btn2 = 0, flag = 0, cnt = 0;

	while (1) {

		if (check_BP1() == 0) {
			btn1 = 1; btn2 = 0;
		}

		if (checkPIT()) {
			cnt++;
		}

		if (cnt == DELAY) {

			if (btn1 == 0 && btn2 == 0) {
				if (seconds == 9) {
					seconds = 1;
				}
				else seconds++;

				if (flag == 0) {
					turn_on_LED1();
					flag = 1;
				}
				else {
					turn_off_LED1();
					flag = 0;
				}
			}

			cnt = 0;
		}

		if (btn1 == 1 && btn2 == 0)
		{
			seconds = 0;
			btn1 = 0; btn2 = 0;
		}


		if (check_BP2() == 0) {
			btn1 = 0; btn2 = 1;
		}

	}

	return 0;
}

