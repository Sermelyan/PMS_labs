	PRESERVE8							; 8-битное выравнивание стека
	THUMB								; Режим Thumb (AUL) инструкций

	GET	stm32f10x.s	

	AREA RESET, CODE, READONLY

	; Таблица векторов прерываний
	DCD 0x20000100						; Указатель на вершину стека
	DCD Reset_Handler					; Вектор сброса
	; ...
	; Тут должны быть векторы прерываний
	; ...
	
	ENTRY								; Точка входа в программу

Reset_Handler	PROC    ; Вектор сброса
    export Reset_Handler
	MOV32	R0, PERIPH_BB_BASE + \
			(RCC_APB2ENR-PERIPH_BASE) * 32 + \
			2 * 4						; вычисляем адрес для BitBanding 5-го бита регистра RCC_APB2ENR
										; BitAddress = BitBandBase + (RegAddr * 32) + BitNumber * 4
	MOV		R1, #1						; включаем тактирование порта D (в 5-й бит RCC_APB2ENR пишем '1`)
	STR 	R1, [R0]					; загружаем это значение
	
	MOV32	R0, GPIOA_CRH			    ; адрес порта
	MOV		R1, #2_0100					; 4-битная маска настроек для Output mode 50mHz, Push-Pull ("0011")
	LDR		R2, [R0]					; считать порт
    BFI		R2, R1, #4, #4    			; скопировать биты маски в позицию PIN7
    BFI		R2, R1, #8, #4    			; скопировать биты маски в позицию PIN7
    BFI		R2, R1, #12, #4    			; скопировать биты маски в позицию PIN7
	BFI		R2, R1, #16, #4    			; скопировать биты маски в позицию PIN7
    STR		R2, [R0]					; загрузить результат в регистр настройки порта
    
    MOV32	R0, PERIPH_BB_BASE + \
			(RCC_APB2ENR-PERIPH_BASE) * 32 + \
			4 * 4						; вычисляем адрес для BitBanding 5-го бита регистра RCC_APB2ENR
										; BitAddress = BitBandBase + (RegAddr * 32) + BitNumber * 4
	MOV		R1, #1						; включаем тактирование порта D (в 5-й бит RCC_APB2ENR пишем '1`)
	STR 	R1, [R0]					; загружаем это значение
    
    MOV32	R0, GPIOC_CRH		     	; адрес порта
	MOV		R1, #2_0011					; 4-битная маска настроек для Output mode 50mHz, Push-Pull ("0011")
	LDR		R2, [R0]					; считать порт
    BFI		R2, R1, #20, #4    			; скопировать биты маски в позицию PIN7
    STR		R2, [R0]

    MOV32	R0, GPIOC_BSRR              ; адрес порта выходных сигналов
    MOV32	R1, GPIOA_IDR               ; адрес порта выходных сигналов

loop									; Бесконечный цикл
    BL      get_button
    
    CMP     R4, #0
    BEQ     loop
    
    MOV     R5, 0x1000
    MUL     R5, R4, R5
    
    MOV32 	R6, GPIO_BSRR_BS13	        ; устанавливаем вывод в '1'
	STR 	R6, [R0]					; загружаем в порт
	
	BL		delay						; задержка
	
	MOV32	R6, GPIO_BSRR_BR13			; сбрасываем в '0'
	STR 	R6, [R0]					; загружаем в порт
	
	MOV     R5, 0x1000
    MUL     R5, R4, R5
	
	BL		delay						; задержка

	B 		loop						; возвращаемся к началу цикла
	
	ENDP

delay		PROC						; Подпрограмма задержки
	SUBS	R5, #1	    ; SUB с установкой флагов результата
    IT      NE
    BNE		delay    					; переход, если Z==0 (результат вычитания не равен нулю)
	BX		LR							; выход из подпрограммы (переход к адресу в регистре LR - вершина стека)
	ENDP
	
get_button  PROC
    LDR     R2, [R1]
    
    ANDS    R3, R2, GPIO_IDR_IDR9
    ITT     EQ
    MOVEQ   R4, #1
    BXEQ    LR
    
    ANDS    R3, R2, GPIO_IDR_IDR10
    ITT     EQ
    MOVEQ   R4, #2
    BXEQ    LR

    ANDS    R3, R2, GPIO_IDR_IDR11
    ITT     EQ
    MOVEQ   R4, #3
    BXEQ    LR
    
    ANDS    R3, R2, GPIO_IDR_IDR12
    ITT     EQ
    MOVEQ   R4, #4
    BXEQ    LR
    
    MOV     R4, #0
    BX		LR
    
    ENDP

    END
