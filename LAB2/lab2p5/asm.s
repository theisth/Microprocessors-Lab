/*
	Ahmet Ali Tilkicioglu
	Burak Balkan
	İsmail Ağca
*/

// Dark Knight


.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOB_BASE,       (0x50000400)          // GPIOB base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOB ODR register offset
.equ GPIOB_IDR,        (GPIOB_BASE + (0x10)) // GPIOB IDR register offset

/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss
	 * not necessary for rom only code
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
	/* enable GPIOB clock, bit2 set on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]

	movs r4, 0x2
	orrs r5, r5, r4
	str r5, [r6]

	/* setup from PB0 to PB7 output for leds with MODER */
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]

	ldr r4, =0xFFFF
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x5555
	orrs r5, r5, r4
	str r5, [r6]

start: // for first two action
	/* turn on leds connected to B port with ODR */
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r4, =0x80 // r4 = 1000 0000
	orrs r5, r5, r4
	str r5, [r6]

	ldr r0,=#1000000 //reload
	bl delay //0.125 sec delay

	ldr r7,=0x80

	lsrs r4, r4, #1 //shift right
	orrs r4, r4, r7
	bics r5, r5, r5 //reset

	orrs r5, r5, r4
	str r5, [r6] //r5 = 1100 0000 load

	ldr r0,=#1000000 //reload
	bl delay //0.125 sec delay


	lsrs r4, r4, #1 //shift right
	orrs r4, r4, r7
	bics r5, r5, r5 //reset

	orrs r5, r5, r4
	str r5, [r6] // r5 = 1100 0000 load

	bics r5, r5, r5 //reset again before enter loop

	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r4, =0xE0 // r4 = 1110 0000 set again bits
	orrs r5, r5, r4
	str r5, [r6]

	ldr r0,=#1000000 //reload
	bl delay //0.125 sec delay

	ldr r1, =0x07 // r1 = 0000 0111
	ldr r2, =0xE0 // r2 = 1110 0000

	b Right_time

Right_time: //right side function
	cmp r4 , r1 //if r4 == 0000 0111
	beq Left_time

	lsrs r4, r4, #1 //shift right
	bics r5, r5, r5

	orrs r5, r5, r4
	str r5, [r6]

	ldr r0,=#1000000 //reload
	bl delay // 0.125 sec. delay
	b Right_time

Left_time: //left time funciton
	cmp r4 , r2 // if r4 == 1110 0000
	beq Right_time

	lsls r4, r4, #1
	bics r5, r5, r5

	orrs r5, r5, r4
	str r5, [r6]

	ldr r0,=#1000000 //reload
	bl delay // 0.125 sec. delay
	b Left_time


delay:
	subs r0,#1 // x = x - 1
	bne delay
	bx lr



	/* for(;;); */
	b .

	/* this should never get executed */
	nop
