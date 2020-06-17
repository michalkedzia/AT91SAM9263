.global main
.arm

.EQU PIOB_PER, 0xFFFFF400        
.EQU PIOB_OER, 0xFFFFF410
.EQU PIOB_SODR, 0xFFFFF430
.EQU PIOB_CODR, 0xFFFFF434
.EQU PIOB_PDSR, 0xFFFFF43C

.EQU PIOC_PER, 0xFFFFF600        
.EQU PIOC_OER, 0xFFFFF610
.EQU PIOC_SODR, 0xFFFFF630
.EQU PIOC_CODR, 0xFFFFF634

.EQU PIOC_ODR, 0xFFFFF614
.EQU PIOC_PDR, 0xFFFFF604
.EQU PIOC_PDSR, 0xFFFFF63C
.EQU PIOC_PUER, 0xFFFFF664
.EQU PMC_PCER, 0xFFFFFC10

.EQU LED_ONE, 0x00000100        
.EQU LED_TWO, 0x20000000        
.EQU BTN_ONE, 0x00000020        
.EQU BTN_TWO, 0x00000010        
.EQU CLOCK, 0x00000010    


.EQU BLINK_DELAY, 3000000


.section .text
main:
    
LDR R2,=LED_TWO
LDR R4,=BTN_TWO

LDR R5,=PIOC_PER
MOV R6, #0
ADD R6, R2,r4
STR R6,[R5]

LDR R5,=PIOC_OER
STR R2,[R5]

LDR R5, =PMC_PCER
LDR R6, =CLOCK
STR R6,[R5]

MOV R5 , #0
LDR R6, =PIOC_ODR
LDR R7, =PIOC_PUER
STR R4,[R6]
STR R4,[R7]




loop:

    LDR R0,=BLINK_DELAY
    delay_on:
    bl turn_on_led_two
    subs r0,r0,#1
    bne delay_on


    LDR R0,=BLINK_DELAY
    delay_off:
    bl check_btn
    subs r0,r0,#1
    bne delay_off

    b loop


check_btn:
    LDR R1, =BTN_TWO
    LDR R2, =PIOC_PDSR
    LDR R3, [R2]
    MOV R4, #0 
    ANDS R4,R1,R3
    STMFD sp!, {LR} 
    BLNE turn_off_led_two
    BLEQ turn_on_led_two
    LDMFD sp!, {pc}
 

    
turn_on_led_two:
    LDR R1,=LED_TWO
    LDR R2, =PIOC_CODR
    STR R1,[R2]
    MOV PC,LR

turn_off_led_two:
    LDR R1,=LED_TWO
    LDR R2, =PIOC_SODR
    STR R1,[R2]
    MOV PC,LR







