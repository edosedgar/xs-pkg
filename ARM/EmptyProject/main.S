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
.equ RCC, 			0x40023800
.equ AHB1ENR,		0x30
.equ GPIOD_EN,		0x08

// GPIOD
.equ GPIOD,			0x40020C00
.equ MODER,			0x00
.equ ODR, 			0x14
.equ LedRed,		(1<<14)
.equ LedGreen,		(1<<12)
.equ LedOrange,		(1<<13)
.equ LedBlue,		(1<<15)
.equ OutMode,		0x55555555


.global Reset_Handler
.type Reset_Handler, function

Reset_Handler:	

	MOVW R0, 0x0001 
	MOVT R0, 0x0000
	MOVW R1, 20		
	MOVT R1, 0x2000	
	STR R0, [R1]	
	
	MOVW R0, 30	    
	MOVT R0, 0x2000

	MOVW R1, 20	
	MOVT R1, 0x2000	
	LDR R1, [R1]	

	STR R1, [R0]		


	
	/*MOVW R0, (RCC+AHB1ENR)&0x0000FFFF
	MOVT R0, (RCC+AHB1ENR)>>16
	MOVW R1, GPIOD_EN
	STR R1, [R0]

	MOVW R0, (GPIOD+MODER)&0x0000FFFF
	MOVT R0, (GPIOD+MODER)>>16
	MOVW R1, (OutMode)&0x0000FFFF //Set all as digital output
	MOVT R1, (OutMode)>>16
	STR R1, [R0]
	
	
  	MOVW R0, (GPIOD+ODR)&0x0000FFFF
	MOVT R0, (GPIOD+ODR)>>16
	MOVW R1, (0xFFFFFFFF)&0x0000FFFF //Set all as digital output
	MOVT R1, (0xFFFFFFFF)>>16
	
	
	MOVW R2, 0x0000
	MOVT R2, 0x2000
	STR R1, [R2]
	
	LDR R2, [R2]
	
	STR R2, [R0]
	
Main:
	B Main*/
	
	//MOV R0, 16386 
	
	/*MOVT R0, 0	
	MOVW R0, 16386 	
	MOVT R1, 0x2000	
	MOVW R1, 0x0000		
	LDR R0, [R1]		

	//PUSH R0
	
	MOVT R0, 0x2000
	MOVW R0, 0x0000	
	LDR R0, [R0]	
	PUSH {R0}
	
	//MOV R0, 14384
	
	MOVT R0, 0	
	MOVW R0, 14384 	
	MOVT R1, 0x2000	
	MOVW R1, 0x0000		
	LDR R0, [R1]	
	
	//PUSH R0
	
	MOVT R0, 0x2000
	MOVW R0, 0x0000	
	LDR R0, [R0]	
	PUSH {R0}
	
	//MOV R1, 8
	
	MOVT R0, 0	
	MOVW R0, 8 	
	MOVT R1, 0x2000	
	MOVW R1, 0x0001
	LDR R0, [R1]	
	
	//OUT R1
	
	POP {R0} 
	POP {R2}
	LSL R2,R2,16
	ADD R2,R2,R0
	MOVT R0, 0x2000
	MOVW R0, 0x0001
	LDR R0, [R0]
	STR R0, [R2]
	
	//
	
	//MOV R0, 16386 

	MOVT R0, 0	
	MOVW R0, 16386 	
	MOVT R1, 0x2000	
	MOVW R1, 0x0000		
	LDR R0, [R1]	
	
	//PUSH R0
	
	MOVT R0, 0x2000
	MOVW R0, 0x0000	
	LDR R0, [R0]	
	PUSH {R0}
	
	//MOV R0, 3074
	
	MOVT R0, 0	
	MOVW R0, 3072 	
	MOVT R1, 0x2000	
	MOVW R1, 0x0000		
	LDR R0, [R1]		
	
	//PUSH R0
	
	MOVT R0, 0x2000
	MOVW R0, 0x0000	
	LDR R0, [R0]	
	PUSH {R0}
	
	//MOV R1, 21845
	
	MOVT R0, 0x5555	
	MOVW R0, 0x5555 	
	MOVT R1, 0x2000	
	MOVW R1, 0x0001		
	LDR R0, [R1]	
	
	//OUT R1
	
	POP {R0} 
	POP {R2}
	LSL R2,R2,16
	ADD R2,R2,R0
	MOVT R0, 0x2000
	MOVW R0, 0x0001
	LDR R0, [R0]
	STR R0, [R2]
	
	//
	  

	//MOV R0, 16386 
		
	MOVW R0, 0x4002 
	MOVT R0, 0
	MOVW R1, 0x0000	
	MOVT R1, 0x2000		
	STR R0, [R1]	
	
	//PUSH R0
	
	MOVW R0, 0x0000	
	MOVT R0, 0x2000
	LDR R0, [R0]	
	PUSH {R0}
	
	//MOV R0, 3094
	
	MOVW R0, 0x0C14 
	MOVT R0, 0	
	MOVW R1, 0x0000		
	MOVT R1, 0x2000	
	STR R0, [R1]	
	
	//PUSH R0
	
	MOVW R0, 0x0000	
	MOVT R0, 0x2000
	LDR R0, [R0]	
	PUSH {R0}
	
	//MOV R1, 61440
	
	MOVW R0, 0xFFFF
	MOVT R0, 0x0000	
	MOVW R1, 0x0002	
	MOVT R1, 0x2000	
	STR R0, [R1]	
	
	//OUT R1
	
	POP {R0} 
	POP {R2}
	LSL R2,R2,16
	ADD R2,R2,R0
	MOVW R0, 0x0002
	MOVT R0, 0x2000
	LDR R0, [R0]
	STR R0, [R2]
		
	//;178.163.25.200
	
	
	
	MOVW R0, 0x0C14 
	MOVT R0, 0x4002
	MOVW R1, 0x0000		
	MOVT R1, 0x2000	
	STR R0, [R1]	

	MOVW R0, 0x0000
	MOVT R0, 0x2000	
	LDR R0, [R0]		
	PUSH {R0}	

	MOVW R0, 0xF000 
	MOVT R0, 0x0000
	MOVW R1, 0x000A		
	MOVT R1, 0x2000	
	STR R0, [R1]	

	POP {R0}
	MOVW R1, 0x000A
	MOVT R1, 0x2000
	LDR R1, [R1]
	STR R1, [R0]*/
		


		

		


		

    
