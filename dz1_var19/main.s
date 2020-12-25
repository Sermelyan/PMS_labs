	PRESERVE8
	THUMB

	AREA DZ1, CODE, READONLY

	DCD 0x20000100
	DCD Reset_Handler
	
	ENTRY
Reset_Handler PROC
	export Reset_Handler
	ldr R0, =arr1
	ldr R1, =5
	bl	crc8
	
	ldr R0, =arr2
	ldr R1, =6
	bl	crc8
	
	ENDP

crc8 PROC
	ldr R2, =0xff
	
outter
	ldrb R3, [R0], #1
	eor R2, R2, R3
	ldr R4, =8
	
inner
	lsl R6, R2, #1
	ands R5, R2, #0x80
	ite ne
	eorne R2, R6, #0x31
	moveq R2, R6
	
	subs R4, #1
	bne inner
	
	subs R1, #1
	bne outter
	
	bx LR
	
	ENDP

	AREA SRC, DATA, READONLY
	
arr1	DCB 1,2,3,4,5,6,7,8,9
arr2	DCB "hello"
arr3	DCB 0,12,145,255,13,1
		
	END