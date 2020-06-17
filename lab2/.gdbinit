# VirtualLab DMCS@2020
set auto-load safe-path /
#/home/dmakow/sw2020/at91sam9263/virtual_lab/.gdbinit

set confirm off
def r
run
end

def run
load
c
end

define nni
ni
disass $pc,+16
end

define ssi
si
disass $pc,+16
end

set confirm on

set remotetimeout 10

target remote :1234
b main
