	PRESERVE8
	THUMB

	AREA DZ1, CODE, READONLY

	DCD 0x20000100
	DCD Reset_Handler
	
	ENTRY
Reset_Handler PROC
	export Reset_Handler
	ldr R0, =arr1
	ldr R1, =9
	bl	crc8
	
	ldr R0, =arr2
	ldr R1, =5
	bl	crc8
	
	ldr R0, =arr3
	ldr R1, =8
	bl	crc8
	
	b loop
	
	ENDP
		
loop PROC
	b loop
	
	ENDP

crc8 PROC
	ldr R2, =0x0
	
outter
	ldrb R3, [R0], #1
	eor R2, R2, R3
	ldr R3, =8
	
inner
	lsl R4, R2, #1
	ands R5, R2, #0x80
	ite ne
	eorne R2, R4, #0xd5
	moveq R2, R4
	
	subs R3, #1
	bne inner
	
	subs R1, #1
	bne outter
	
	bfc R2, #8, #24
	bx LR
	
	ENDP

	AREA SRC, DATA, READONLY
	
arr1	DCB "123456789"
arr2	DCB "hello"
arr3	DCB "Melikyan"
		
	END