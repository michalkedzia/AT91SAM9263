# SW VirtualLab DMCS@2020

CCFLAGS=-g -mcpu=arm9 -Os -Wall
CC=arm-none-eabi-gcc
#LDFLAGS+=-nostartfiles -Wl,--cref -lc -lgcc -T /opt/arm_user/elf32-littlearm.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
LDFLAGS+=-nostartfiles -W -lc -lgcc -T ~/sw/at91sam9263/virtual_lab/script.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
OBJS=~/sw/at91sam9263/virtual_lab/cstartup.o ~/sw/at91sam9263/virtual_lab/lowlevel.o
#koniec, dalej mozna dowolnie zmieniac

PROGRAM_NAME=lab6.elf

all: $(PROGRAM_NAME)

rebuild: clean $(PROGRAM_NAME)

$(PROGRAM_NAME): main.o
	$(CC) $(LDFLAGS) $(OBJS) $< -o $@

main.o: main.c 
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm -f *.o *.elf 
