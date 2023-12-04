/*
	Hazirlayan:
 	 Ahmet Ali Tilkicioglu (the1sth)
*/

//4-Digit Seven Segment Register Application

#include "stm32g0xx.h"

void systickInit(void);			// Systick Init.
void SysTick_Handler(void);		// Systick Handler
void delay_ms(int ms);			// Delay Function
void num_zero(int t);			// Seven Segment Number 0
void num_one(int t);			// Seven Segment Number 1
void num_two(int t);			// Seven Segment Number 2
void num_three(int t);			// Seven Segment Number 3
void num_four(int t);			// Seven Segment Number 4
void num_five(int t);			// Seven Segment Number 5
void num_six(int t);			// Seven Segment Number 6
void num_seven(int t);			// Seven Segment Number 7
void num_eight(int t);			// Seven Segment Number 8
void num_nine(int t);			// Seven Segment Number 9
void led4(int a); 				// 4 Digit 7 Segment LED4 Number Changer
void led3(int b); 				// 4 Digit 7 Segment LED3 Number Changer
void led2(int c); 				// 4 Digit 7 Segment LED2 Number Changer
void led1(int d); 				// 4 Digit 7 Segment LED1 Number Changer
void GPIO_config(void); 		// GPIO Configuration
void action(void);				// Base Code
void onFinish_led(void);		// Finish LED On Mode
void offFinish_led(void);		// Finish LED On Mode
void RCC_config(void);			// RCC Configuration

int digit4;						// Digit 4 Counter
int digit3;						// Digit 3 Counter
int digit2;						// Digit 2 Counter
int digit1;						// Digit 1 Counter
int millis;						// Systick Counter
int counter;					// Step Counter
int stop_ct = 0;				// Stop Counter
int flag = 0;					// Finish Flag



int main(void) {

	RCC_config();
	GPIO_config();
	systickInit();
	EXTI_config();


    while(1) {

    	while(stop_ct < 1000)
    	{
    		action(); // main code
    	}
    }

    return 0;
}




void SysTick_Handler(void) // Interrupt function
{
	millis++;      // millis increase +1
}

void delay_ms(int ms)  // ms delay function
{
	int delay = millis+ms;
	while(millis<delay);
}

void systickInit(void)
{

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 	// counter activated
	SysTick->LOAD = 49;                     	// Reload value
	SysTick->VAL=0;                        		// reset counter
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // interrupt activated
	SysTick->CTRL &= ~(1<<2);                   // Systick clock source = AHB/8
	NVIC_EnableIRQ(SysTick_IRQn);     			// NVIC activated
	NVIC_SetPriority (SysTick_IRQn,0);  		// Priority set

}

void GPIO_config(void) // GPIO configuration function
{

	/* Setup PA1 as output */
	GPIOA->MODER &= ~(3U << 2*1);
	GPIOA->MODER |= (1U << 2*1);
	/* Setup PA4 as output */
	GPIOA->MODER &= ~(3U << 2*4);
	GPIOA->MODER |= (1U << 2*4);
	/* Setup PA5 as output */
	GPIOA->MODER &= ~(3U << 2*5);
	GPIOA->MODER |= (1U << 2*5);
	/* Setup PA6 as output */
	GPIOA->MODER &= ~(3U << 2*6);
	GPIOA->MODER |= (1U << 2*6);


	/* Setup PB0 as output */
	GPIOB->MODER &= ~(3U << 2*0);
	GPIOB->MODER |= (1U << 2*0);
	/* Setup PB1 as output */
	GPIOB->MODER &= ~(3U << 2*1);
	GPIOB->MODER |= (1U << 2*1);
	/* Setup PB2 as output */
	GPIOB->MODER &= ~(3U << 2*2);
	GPIOB->MODER |= (1U << 2*2);
	/* Setup PB3 as input */
	GPIOB->MODER &= ~(3U << 2*3);
	GPIOB->MODER |= (1U << 2*3);
	/* Setup PB4 as output */
	GPIOB->MODER &= ~(3U << 2*4);
	GPIOB->MODER |= (1U << 2*4);
	/* Setup PB5 as output */
	GPIOB->MODER &= ~(3U << 2*5);
	GPIOB->MODER |= (1U << 2*5);
	/* Setup PB6 as output */
	GPIOB->MODER &= ~(3U << 2*6);
	GPIOB->MODER |= (1U << 2*6);
	/* Setup PB8 as input */
	GPIOB->MODER &= ~(3U << 2*8);
	/*Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);


}


void led4(int a)    // LED4 number displayer
{
	int t1 = 100;
	GPIOA->ODR &= (0x00);
	GPIOA->ODR |= (0x40); //D4 active

	switch (a) {
		case 0:
			num_zero(t1);
			break;

		case 1:
			num_one(t1);
			break;

		case 2:
			num_two(t1);
			break;

		case 3:
			num_three(t1);
			break;

		case 4:
			num_four(t1);
			break;

		case 5:
			num_five(t1);
			break;

		case 6:
			num_six(t1);
			break;

		case 7:
			num_seven(t1);
			break;

		case 8:
			num_eight(t1);
			break;

		case 9:
			num_nine(t1);
			break;
	}
}


void led3(int b) // LED3 number displayer
{
	int t1 = 100;
	GPIOA->ODR &= (0x00);
	GPIOA->ODR |= (0x20); //D3 active

	switch (b) {
		case 0:
			num_zero(t1);
			break;

		case 1:
			num_one(t1);
			break;

		case 2:
			num_two(t1);
			break;

		case 3:
			num_three(t1);
			break;

		case 4:
			num_four(t1);
			break;

		case 5:
			num_five(t1);
			break;

		case 6:
			num_six(t1);
			break;

		case 7:
			num_seven(t1);
			break;

		case 8:
			num_eight(t1);
			break;

		case 9:
			num_nine(t1);
			break;
	}
}

void led2(int c) // LED2 number displayer
{
	int t1 = 100;
	GPIOA->ODR &= (0x00);
	GPIOA->ODR |= (0x10); //D2 active

	switch (c) {
		case 0:
			num_zero(t1);
			break;

		case 1:
			num_one(t1);
			break;

		case 2:
			num_two(t1);
			break;

		case 3:
			num_three(t1);
			break;

		case 4:
			num_four(t1);
			break;

		case 5:
			num_five(t1);
			break;

		case 6:
			num_six(t1);
			break;

		case 7:
			num_seven(t1);
			break;

		case 8:
			num_eight(t1);
			break;

		case 9:
			num_nine(t1);
			break;
	}
}

void led1(int d) // LED1 number displayer
{
	int t1 = 100;
	GPIOA->ODR &= (0x00);
	GPIOA->ODR |= (0x02); //D1 active

	switch (d) {
		case 0:
			num_zero(t1);
			break;

		case 1:
			num_one(t1);
			break;

		case 2:
			num_two(t1);
			break;

		case 3:
			num_three(t1);
			break;

		case 4:
			num_four(t1);
			break;

		case 5:
			num_five(t1);
			break;

		case 6:
			num_six(t1);
			break;

		case 7:
			num_seven(t1);
			break;

		case 8:
			num_eight(t1);
			break;

		case 9:
			num_nine(t1);
			break;
	}
}

/* All number display functions */
void num_zero(int t) // 0 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x3F);
}

void num_one(int t) // 1 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x06);
}

void num_two(int t) // 2 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x5B);
}

void num_three(int t) // 3 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x4F);
}

void num_four(int t) // 4 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x66);
}

void num_five(int t) // 5 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x6D);
}

void num_six(int t) // 6 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x7D);
}

void num_seven(int t) // 7 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x07);
}

void num_eight(int t) // 8 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0xFF);
}

void num_nine(int t) // 9 Display
{
    GPIOB->ODR &= (0x00);
    GPIOB->ODR |= ~(0x6F);
}


void action(void) // algorithm main code
{
	offFinish_led();

    if (counter >= 1000)
    {
    	counter = 0;
    	digit4 = digit4 + 1;
    }

    if (digit4 == 10) //reset digit
    {
    	digit4 = 0;
    	digit3 = digit3 + 1;
	    stop_ct = stop_ct + 1;


		if (digit3 == 10) //reset digit
		{
			digit3 = 0;
			digit2 = digit2 + 1;
		}

		if (digit2 == 10) // reset digit
		{
			digit2 = 0;
			digit1 = digit1 + 1;
		}


		if(digit1 == 10) //reset digit
		{
			digit1 = 0;

		}
	}

    /* Display the number */
    led1(digit1);
    delay_ms(1);
    led2(digit2);
    delay_ms(1);
    led3(digit3);
    delay_ms(1);
    led4(digit4);
    delay_ms(1);
    counter = counter + 100;
    flag = flag + 1;


    if(flag == 99999) // last number holder
    {
    	while(1)
    	{
    	    led1(digit1);
    	    delay_ms(1);
    	    led2(digit2);
    	    delay_ms(1);
    	    led3(digit3);
    	    delay_ms(1);
    	    led4(digit4);
    	    delay_ms(1);
    	    flag = 0;
    	    onFinish_led();
    	    if(stop_ct == 0)
    	    	break;
    	}
    }


}


void EXTI_config(void) // EXTI configuration function
{
	 EXTI->EXTICR[2] |= (1U << 8*0); 	//PB8 EXTI
	 EXTI->RTSR1 |= (1U << 8); 			//Rising Edge PB8
	 EXTI->IMR1 |= (1U << 8); 			//Mask for PB8
	 NVIC_SetPriority(EXTI4_15_IRQn, 0);
	 NVIC_EnableIRQ(EXTI4_15_IRQn);
}


void EXTI4_15_IRQHandler(void)
{
	/* Reset the System */
	digit1 = 0;
	digit2 = 0;
	digit3 = 0;
	digit4 = 0;
	counter = 0;
	stop_ct = 0;
	flag = 0;

	EXTI->RPR1 |= (1U << 8);
}


void onFinish_led(void)
{
    GPIOC->ODR |= (1U << 6); 			// PC6 Board LED On
}


void offFinish_led(void)
{
    GPIOC->ODR &= (0x00); 				// PC6 Board LED Off
}

void RCC_config(void)
{
	/* Clock A,B and C activated */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);
    RCC->IOPENR |= (1U << 2);
}
