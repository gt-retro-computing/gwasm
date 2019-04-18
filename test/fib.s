;fib
.org 0x0
init:
	;Test Noarg
	sphl
	mvi a, 0
	mvi b, 1
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
