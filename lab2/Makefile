# Common definitions

ASFLAGS=-D__ASSEMBLY__ -g -mcpu=arm9 -c -O0 -Wall
CCFLAGS=-g -mcpu=arm9 -O0 -Wall
CC=arm-none-eabi-gcc
#LDFLAGS+=-nostartfiles -Wl,--cref -lc -lgcc -T /opt/arm_user/elf32-littlearm.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
LDFLAGS+=-nostartfiles -W -lc -lgcc -T ~/sw/at91sam9263/virtual_lab/script.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
EXTOBJS=~/sw/at91sam9263/virtual_lab/cstartup.o ~/sw/at91sam9263/virtual_lab/lowlevel.o 
OBJS=main.o
#koniec, dalej mozna dowolnie zmieniac


# Output file base name
PROGRAM_NAME=lab2

# Main tagret: build the program
all: $(PROGRAM_NAME).elf

# Run the debugger with application
run: $(PROGRAM_NAME).elf
	$(GDB) $<

# Linkingco
$(PROGRAM_NAME).elf: $(EXTOBJS) $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Running assembler
%.o: %.S
	$(CC) $(ASFLAGS) $^ -o $@

# Running C compiler
%.o: %.c
	$(CC) -c $(CCFLAGS) $^ -o $@

# Removing output files
clean:
	rm -f $(OBJS) $(PROGRAM_NAME).elf 

# Declaration of "dummy" targets
.PHONY: all clean run
