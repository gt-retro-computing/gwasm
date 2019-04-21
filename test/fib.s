
TEST .equ 0xff
Zero .equ 0
One .equ 1

;fib
.org 0x0
init:
    sphl
	mvi a, Zero
	mvi b, One
	mvi l, 0xFF
fib:
	mov c, a
	add b
	jc init
	xra l
	out 0xff
	xra l
	mov b, c
	mov h, a
	LXI d, 0xFFFF
delay:
	dcx d
	mov a, d
	ora e
	jnz delay
	mov a, h
	jmp fib
