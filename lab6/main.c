#include "AT91SAM9263.h"
#include "AT91SAM9263-EK.h"
//#define MASK_ASCII 0XFF

unsigned char DBGU_RECIVE() {
	while ((*AT91C_DBGU_CSR & AT91C_US_RXRDY) == 0);
	return *AT91C_DBGU_RHR;
	//return *AT91C_DBGU_RHR & MASK_ASCII;
}

void DBGU_TRANSMIT(unsigned char arg) {
	while ((*AT91C_DBGU_CSR & AT91C_US_TXRDY) == 0);
	*AT91C_DBGU_THR = arg;
}

void init() {
	//AT91C_BASE_DBGU->DBGU_IDR = 0xFFFFFFFF; 
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTRX | AT91C_US_RXDIS; 
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RSTTX | AT91C_US_TXDIS; 
	*AT91C_PIOC_PDR = AT91C_PC30_DRXD | AT91C_PC31_DTXD; 
	*AT91C_PIOC_ASR = AT91C_PC30_DRXD | AT91C_PC31_DTXD;
	AT91C_BASE_DBGU->DBGU_BRGR = AT91B_MASTER_CLOCK / (AT91B_DBGU_BAUD_RATE * 16); 
	AT91C_BASE_DBGU->DBGU_MR = AT91C_BASE_DBGU->DBGU_MR | AT91C_US_CHMODE_NORMAL; 
	//------ bez przerwan
	AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN; 
}

int toupper(int arg) {
	if (arg >= 'a' && arg <= 'z') return  (arg - ('a' - 'A'));
	return arg;
}

int main(void) {

	unsigned char arg = 0;
	init();

	while (1) {
		arg = DBGU_RECIVE();
		DBGU_TRANSMIT(toupper(arg));
	}

	return 0;
}
