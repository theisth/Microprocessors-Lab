/*
	Ahmet Ali Tilkicioglu
	Burak Balkan
	İsmail Ağca
*/


// Dark Knight with button mod changer

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

.equ GPIOB_BASE,       (0x50000400)          // GPIOC base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOC MODER register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOC ODR register offset
.equ GPIOB_IDR,        (GPIOB_BASE + (0x10))

.equ GPIOA_BASE,       (0x50000000)          // GPIOA base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
.equ GPIOA_IDR,        (GPIOA_BASE + (0x10)) // GPIOA IDR register offset

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
	/* enable GPIOB-A clock, bit0 and bit1 set on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]

	movs r4, 0x3
	orrs r5, r5, r4
	str r5, [r6]

	// PA1 input 00 with MODER (button)
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	ldr r4, =0x3
	lsls r4, r4, #2
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]

	// setup from PB0 to PB7 as output
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]

	ldr r4, =0xFFFF
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x5555
	orrs r5, r5, r4
	str r5, [r6]

	ldr r6, =GPIOB_ODR
	ldr r1, = #0xE0 // 1110 0000
	str r1, [r6]
	ldr r0, = #0x1 // starting condition


button_ctrl:
	// PA1 button connection
	ldr r6, =GPIOA_IDR
	ldr r5, [r6]
	lsrs r5, r5, #1 // logic shift right 1 bit
	movs r4, #0x1 //r4=0x1
	ands r5, r5, r4 // GPIOA_IDR and r4

	cmp r5, #0x1 // if r5 == 1
	beq change

	cmp r0, #0x1
	beq left // left side function
	bne right // right side function

change: // direction function
	cmp r0, #0x1 // if r0 == 1
	beq if
	bne else

if:
	ldr r0, = #0x0
	b button_ctrl

else:
	ldr r0, = #0x1
	b button_ctrl


right:

	movs r3, #0x1 //0001
	movs r2, r1
	ands r2, r2, r3
	lsls r2, r2, #7 // 7 shift to left
	lsrs r1, r1, #1 // 1 shift to right
	movs r4, #0x1
	lsls r4, r4, #7	// 7 shift to left
	bics r1, r1, r4	// 1 shift to right
	orrs r1, r1, r2
	ldr r6, =GPIOB_ODR
	str r1, [r6]
	ldr r5,=#1000000 // reload
	bl delay	// 0.125 sec. delay
	b button_ctrl

left:

	lsls r2, r1, #1
	lsrs r1, r1, #7 //7 shift to right
	movs r3, #0x1
	ands r1, r1, r3
	orrs r1, r2, r1
	ldr r6, =GPIOB_ODR
	str r1, [r6]
	ldr r5,=#1000000 //reload
	bl delay // 0.125 sec. delay
	b button_ctrl

delay: //delay
	subs r5,r5,#1 //x = x - 1
	bne delay
	bx lr
	nop
