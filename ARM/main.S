    .syntax unified
    .cpu cortex-m4
	.thumb

.equ FlashStart,	0x08000000
.equ FlashSize,   	0x000FFFFF //1024 kB
.equ RAMStart,   	0x20000000 
.equ RAMSize,     	0x0001BFFF //112 kB

.equ Default_Handler, 0

.section .isr // Interrupt table

	.word	RAMStart+RAMSize
    .word	Reset_Handler
	.word   Default_Handler //NMI
	.word   Default_Handler //Hardfault
	.word   Default_Handler //MemManage
	.word   Default_Handler //BusFault
	.word   Default_Handler //UsageFault
	.word   0
	.word   Default_Handler //SVCall
	.word   Default_Handler //Debug Monitor
	.word   0
	.word   Default_Handler //PendSV
	.word   Default_Handler //SysTick

	.word   Default_Handler //WWDG
	.word   Default_Handler //PVD
	.word   Default_Handler //TAMP_STAMP
	.word   Default_Handler //RTC_WKUP
	.word   Default_Handler //FLASH
	.word   Default_Handler //RCC
	.word   Default_Handler //EXTI0
	.word   Default_Handler //EXTI2
	.word   Default_Handler //EXTI3
	.word   Default_Handler //EXTI4
	.word   Default_Handler //DMA1_Stream0
	.word   Default_Handler //DMA1_Stream1
	.word   Default_Handler //DMA1_Stream2
	.word   Default_Handler //DMA1_Stream3
	.word   Default_Handler //DMA1_Stream4
	.word   Default_Handler //DMA1_Stream5
	.word   Default_Handler //DMA1_Stream6
	.word   Default_Handler //ADC
	.word   Default_Handler //CAN1_TX
	.word   Default_Handler //CAN1_RX0
	.word   Default_Handler //CAN1_RX1
	.word   Default_Handler //CAN1_SCE
	.word   Default_Handler //EXTI9_5
	.word   Default_Handler //TIM1_BRK_TIM9
	.word   Default_Handler //TIM1_UP_TIM10
	.word   Default_Handler //TIM1_TRG_COM_TIM11
	.word   Default_Handler //TIM1_CC
	.word   Default_Handler //TIM2
	.word   Default_Handler //TIM3
	.word   Default_Handler //TIM4
	.word   Default_Handler //I2C1_EV
	.word   Default_Handler //I2C1_ER
	.word   Default_Handler //I2C2_EV
	.word   Default_Handler //I2C2_ER
	.word   Default_Handler //SPI1
	.word   Default_Handler //SPI2
	.word   Default_Handler //USART1
	.word   Default_Handler //USART2
	.word   Default_Handler //USART3
	.word   Default_Handler //EXTI5_10
	.word   Default_Handler //RTC_Alarm
	.word   Default_Handler //OTG_FS WKUP
	.word   Default_Handler //TIM8_BRK_TIM12
	.word   Default_Handler //TIM8_UP_TIM13
	.word   Default_Handler //TIM8_TRG_COM_TIM14
	.word   Default_Handler //TIM8_CC
	.word   Default_Handler //DMA1_Stream7
	.word   Default_Handler //FSMC
	.word   Default_Handler //SDIO
	.word   Default_Handler //TIM5
	.word   Default_Handler //SPI3
	.word   Default_Handler //UART4
	.word   Default_Handler //UART5
	.word   Default_Handler //TIM6_DAC
	.word   Default_Handler //TIM7
	.word   Default_Handler //DMA2_Stream0
	.word   Default_Handler //DMA2_Stream1
	.word   Default_Handler //DMA2_Stream2
	.word   Default_Handler //DMA2_Stream3
	.word   Default_Handler //DMA2_Stream4
	.word   Default_Handler //ETH
	.word   Default_Handler //ETH_WKUP
	.word   Default_Handler //CAN2_TX
	.word   Default_Handler //CAN2_RX0
	.word   Default_Handler //CAN2_RX1
	.word   Default_Handler //CAN2_SCE
	.word   Default_Handler //OTG_FS
	.word   Default_Handler //DMA2_Stream5
	.word   Default_Handler //DMA2_Stream6
	.word   Default_Handler //DMA2_Stream7
	.word   Default_Handler //USART6
	.word   Default_Handler //I2C3_EV
	.word   Default_Handler //I2C3_ER
	.word   Default_Handler //OTG_HS_EP1_OUT
	.word   Default_Handler //OTG_HS_EP1_IN
	.word   Default_Handler //OTG_HS_WKUP
	.word   Default_Handler //OTG_HS
	.word   Default_Handler //DCMI
	.word   Default_Handler //CRYP
	.word   Default_Handler //HASH_RNG
	.word   Default_Handler //FPU
	.word   Default_Handler //UART7
	.word   Default_Handler //UART8
	.word   Default_Handler //SPI4
	.word   Default_Handler //SPI5
	.word   Default_Handler //SPI6

.section .text //Main code

// RCC Module
.equ RCC, 		0x40023800
.equ AHB1ENR,		0x30
.equ GPIOD_EN,		0x08

// GPIOD
.equ GPIOD,		0x40020C00
.equ MODER,		0x00
.equ ODR, 		0x14
.equ IDR,		0x10
.equ LedRed,		(1<<14)
.equ LedGreen,		(1<<12)
.equ LedOrange,		(1<<13)
.equ LedBlue,		(1<<15)
.equ OutMode,		0x55555555


.global Reset_Handler
.type Reset_Handler, function

Reset_Handler:		

	NOP
	CMP R0, R1
	NOP
	
	MOVW R0, (RCC+AHB1ENR)&0x0000FFFF
	MOVT R0, (RCC+AHB1ENR)>>16
	MOVW R1, GPIOD_EN
	STR R1, [R0]

	MOVW R0, (GPIOD+MODER)&0x0000FFFF
	MOVT R0, (GPIOD+MODER)>>16
	MOVW R1, (OutMode)&0x0000FFFF //Set all as digital output
	MOVT R1, (OutMode)>>16
	STR R1, [R0]

	//********************************************************
	/*// ADD Rn1, Rn2
	MOVT R0, 0x2000 	//11110010 11000010 00000000 00000000 
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, 0x2000 	//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2
	LDR R1, [R1]		//01101000 00001001

	ADD R0, R0, R1		//00011000 01000000

	MOVT R1, 0x2000 	//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//*********************************************************
	// ADD Rn1, Num
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, high(Num)	//11110i10 1100iiii 0iii0001 iiiiiiii 
	MOVW R1, low(Num)	//11110i10 0100iiii 0iii0001 iiiiiiii

	ADD R0, R0, R1		//00011000 01000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//*********************************************************
	// SUB Rn1, Rn2		
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000 
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2
	LDR R1, [R1]		//01101000 00001001
  
	SUB R0, R0, R1		//00011010 01000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//*********************************************************
	// SUB  Rn1, Num	//11110010 11000010 00000000 00000000
	MOVT R0, 0x2000		//11110010 01000000 00000000 address1
	MOVW R0, Rn1		//01101000 00000000
	LDR R0, [R0]		

	MOVT R1, high(Num)	//11110i10 1100iiii 0iii0001 iiiiiiii
	MOVW R1, low(Num)	//11110i10 0100iiii 0iii0001 iiiiiiii

	SUB R0, R0, R1		//00011010 01000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//*********************************************************	
	// MUL Rn1, Rn2
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2
	LDR R1, [R1]		//01101000 00001001

	MUL R0, R0, R1		//11111011 00000000 11110000 00000001

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//********************************************************
	// MUL Rn1, Num		
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, high(Num)	//11110i10 1100iiii 0iii0001 iiiiiiii
	MOVW R1, low(Num)	//11110i10 0100iiii 0iii0001 iiiiiiii

	MUL R0, R0, R1		//11111011 00000000 11110000 00000001

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//********************************************************
	// DIV Rn1, Rn2
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2
	LDR R1, [R1]		//01101000 00001001

	UDIV R0, R0, R1		//11111011 10110000 11110000 11110001

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//********************************************************
	// DIV Rn1, Num 
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, high(Num)	//11110i10 1100iiii 0iii0001 iiiiiiii
	MOVW R1, low(Num)	//11110i10 0100iiii 0iii0001 iiiiiiii

	UDIV R0, R0, R1		//11111011 10110000 11110000 11110001

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//********************************************************
	// MOV Rn1, Rn2 
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOV R2, R0		//01000110 00000010

	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2
	LDR R1, [R1]		//01101000 00001001

	STR R1, [R0]		//01100000 00000001

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2

	STR R2,[R1]		//01100000 00001010
	//********************************************************
	// MOV Rn1, Num
	MOVT R0, high(Num)	//11110i10 1100iiii 0iii0000 iiiiiiii
	MOVW R0, low(Num) 	//11110i10 0100iiii 0iii0000 iiiiiiii
	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	LDR R0, [R1]		//01101000 00001000
	//*******************************************************
	// CMP Rn1, Rn2
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000

	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn2		//11110010 01000000 00000001 address2
	LDR R1, [R1]		//01101000 00001001

	CMP R0, R1		//01000010 10001000
	//*******************************************************
	// CMP Rn1, Num
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000
	
	MOVT R1, high(Num)	//11110i10 1100iiii 0iii0001 iiiiiiii
	MOVW R1, low(Num)	//11110i10 0100iiii 0iii0001 iiiiiiii

	CMP R0, R1		//01000010 10001000
	//******************************************************
	// PUSH Rn1
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0, [R0]		//01101000 00000000
	PUSH {R0}		//10110100 00000001
	//******************************************************
	// POP Rn1
	POP {R0}		//10111100 00000001
	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1
	STR R0, [R1]		//01100000 00001000
	//******************************************************	
	// CALL Label
	BL Label		//11110sii iiiiiiii 11y1xiii iiiiiiii
	// Label=s:a:b:i10:i11  y=(~a)xor(s) x=(~b)xor(s) 
	//*****************************************************
	// JP Label
	B Label			//11100iii iiiiiiii
	//*****************************************************
	// JA Label
	BHI Label		//11011000 iiiiiiii
	//*****************************************************
	// JB Label
	BCC Label		//11010011 iiiiiiii
	//*****************************************************
	// JAE Label
	BCS Label		//11010010 iiiiiiii
	//****************************************************
	// JBE Label		
	BLS Label		//11011001 iiiiiiii
	//*****************************************************
	// JE Label
	BEQ Label		//11010000 iiiiiiii
	//*****************************************************
	// RET
	BX LR			//01000111 01110000
	//*****************************************************
	// OUT Rn1
	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	LDR R0,[R0]		//01101000 00000000
	
	;GPIO_ODR
	MOVT R1, 0x4002		//11110010 11000100 00000001 00000010
	MOVW R1, 0x0C14		//11110110 01000000 01000001 00100100
	STR R0, [R1]		//01100000 00001000
	//*****************************************************
	// IN Rn1
	;GPIO_IDR
	MOVT R1, 0x4002		//11110010 11000100 00000001 00000010
	MOVW R1, 0x0C10		//11110110 01000000 01000001 00100000
	LDR R1, [R1]		//01101000 00001001

	MOVT R0, 0x2000		//11110010 11000010 00000000 00000000
	MOVW R0, Rn1		//11110010 01000000 00000000 address1
	STR R1, [R0]		//01100000 00000001
	//******************************************************
	// CLR Rn1
	MOVT R1, 0x2000		//11110010 11000010 00000001 00000000
	MOVW R1, Rn1		//11110010 01000000 00000001 address1

	MOVT R0, 0		//11110010 11000000 00000000 00000000
	MOVW R0, 0		//11110010 10000000 00000000 00000000
	STR R0, [R1]		//01100000 00001000
	//*****************************************************
	// NOP 
	NOP			//10111111 00000000
	//*****************************************************

Blink:
    B Blink
    
