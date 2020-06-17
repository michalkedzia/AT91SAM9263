#define LED1_PIN (1 << 8)
#define LED2_PIN (1 << 29)

#define BP1_PIN (1 << 5)
#define BP2_PIN (1 << 4)

#define BP1 5
#define BP2 4

#define DELAY  2500000

#define PIOB_PER ((volatile unsigned int * const )0xFFFFF400)
#define PIOB_OER ((volatile unsigned int * const )0xFFFFF410)
#define PIOB_SODR ((volatile unsigned int * const ) 0xFFFFF430)
#define PIOB_CODR ((volatile unsigned int * const ) 0xFFFFF434)
#define PIOB_PDSR ((volatile unsigned int * const ) 0xFFFFF43C) // dodane


#define PIOC_PER ((volatile unsigned int * const ) 0xFFFFF600 )
#define PIOC_ODR ((volatile unsigned int * const )0xFFFFF614)

#define PIOC_OER ((volatile unsigned int * const )0xFFFFF610)
#define PIOC_SODR ((volatile unsigned int * const ) 0xFFFFF630)
#define PIOC_CODR ((volatile unsigned int * const ) 0xFFFFF634)
#define PIOC_PDR ((volatile unsigned int * const )0xFFFFF604 )

// ODCZYT STANU PRZYCISKU
#define PIOC_PDSR ((volatile unsigned int * const )0xFFFFF63C)
//#define PIOC_ODR ((volatile unsigned int * const )0xFFFFF614)
#define PIOC_PUER ((volatile unsigned int * const ) 0xFFFFF664)

// JEDEN REJSTR DO WSZYSTKICH
#define PMC_PCER  ((volatile unsigned int * const ) 0xFFFFFC10)
#define Peripheral_ID_C (1 << 4)

void init_LED1(){
	*PIOB_PER = LED1_PIN;
	*PIOB_OER = LED1_PIN;
	*PIOB_SODR = LED1_PIN;
}

void init_LED2(){
	*PIOC_PER = LED2_PIN;
	*PIOC_OER = LED2_PIN;
	*PIOC_SODR = LED2_PIN;
}

void enable_clock(){
	*PMC_PCER = Peripheral_ID_C;
}

void init_BP1(){
	*PIOC_PER = BP1_PIN;
	*PIOC_ODR = BP1_PIN;
	*PIOC_PUER = BP1_PIN;
}

void init_BP2(){
	*PIOC_PER = BP2_PIN;
	*PIOC_ODR = BP2_PIN;
	*PIOC_PUER = BP2_PIN;
}

void turn_on_LED1(){
	*PIOB_CODR = LED1_PIN;
}

void turn_on_LED2(){
	*PIOC_CODR = LED2_PIN;
}

void turn_off_LED1(){
	*PIOB_SODR = LED1_PIN;
}

void turn_off_LED2(){
	*PIOC_SODR = LED2_PIN;
}

int check_BP1(){
	return  (((*PIOC_PDSR) >> BP1) & 1);
}

int check_BP2(){
	return  (((*PIOC_PDSR) >> BP2) & 1);
}

void delay(long long int time_delay){
	for (volatile long long int i = 0; i < 100000 * time_delay; i++);
}

int main(void) {
	init_LED1();
	init_LED2();
	enable_clock();
	init_BP1();
	init_BP2();

	while (1)
	{

		turn_on_LED1();

		for (volatile long long int i = 0; i < DELAY; i++)
		{

			if (check_BP1() == 0)
				turn_on_LED2();

			if (check_BP2() == 0)
				turn_off_LED2();

		}

		turn_off_LED1();

		for (volatile long long int i = 0; i < DELAY; i++)
		{

			if (check_BP1() == 0)
				turn_on_LED2();

			if (check_BP2() == 0)
				turn_off_LED2();

		}
	}


	return 0;
}
