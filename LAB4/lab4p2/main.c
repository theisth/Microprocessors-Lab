/*
 * Hazirlayanlar:
 * 		Ahmet Ali Tilkicioglu
 * 		Burak Balkan
 * 		İsmail Ağca
 */

#include "stm32g0xx.h"

void init_pwm2(void);
void PWM_Value();

uint16_t pwm_value = 0;
uint8_t counter = 0;

int main(void)
{
	init_pwm2();

	while(1)
	{

	}

	return 0;
}



//PA1-CH2-TIM2-AF2
void init_pwm2(void)
{
	RCC->IOPENR |= (1U << 0); //Enable clock for GPIOA
	//PA1 alernate func.
	GPIOA->MODER &= ~(3U << 2*1);
	GPIOA->MODER |= (2U << 2*1);
	//CH2 timer selected
	GPIOA->AFR[0] &= ~(0xFU << 4*1);
	GPIOA->AFR[0] |= (2U << 4*1); //
	//TIM2 Enable
	RCC->APBENR1 |= (1U << 0);

	TIM2-> CR1 |= 0 << 4; // Up counter en.
	TIM2-> CR1 |= 0 << 5; // center-aligned mode
	TIM2-> CR1 |= 0 << 8; // timer clock division 1

	TIM2->CCMR1 |= 1 << 11; // output compare 2
	TIM2->CCMR1 |= 0 << 8;  // CC2 channel output
	TIM2->CCMR1 |= 6 << 12; //pwm mode 1

	TIM2->CCER |= 1 << 4; // capture compare output enable

	TIM2->PSC = 10;	// prescaler
	TIM2->ARR = 16000; // %100 duty cycle
	TIM2->CCR2 = 0;		//first value 0/16000

	TIM2->DIER |= (1 << 0); //update interrupt

	TIM2->CR1 |= 1 << 0; //enable pwm

	NVIC_SetPriority(TIM2_IRQn,1);
	NVIC_EnableIRQ(TIM2_IRQn);

}


void PWM_Value(){

	if(pwm_value < 16000 && counter == 0)
	{
		pwm_value  = pwm_value + 200;
		counter = 0;
	    if(pwm_value >=16000)
			counter = 1;
	}

	if(counter == 1)
	{
		pwm_value = pwm_value - 200;
		counter = 1;
	    if(pwm_value == 0)
	    	counter = 0;
	}
}


void TIM2_IRQHandler()
{
	PWM_Value();	//pwm value function
	TIM2-> CCR2 = pwm_value; //duty cycle setting
}
