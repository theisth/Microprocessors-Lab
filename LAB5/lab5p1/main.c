/*
 * Hazirlayanlar:
 * 		Ahmet Ali Tilkicioglu
 * 		Burak Balkan
 * 		İsmail Ağca
 */

#include "stm32g0xx.h"


uint16_t ADC_value = 0;
float pwm_value;
float pwm_value_2;
float k_const = 3.90625;

void GPIO_config(void);
void ADC_config(void);
void ADC_read(void);
void PWM_Value();
void init_pwm2(void);
void init_pwm3(void);

int main(void) {

	//init. all pin and timer
	GPIO_config();
	ADC_config();
	init_pwm2();

    while(1)
    {
    	ADC_read();									// Get ADC value
    	PWM_Value();								// Design PWM values
    	TIM3->CCR1 = pwm_value;						// TIM4 Channel 1 pulse value
    	TIM3->CCR2 = pwm_value_2;					// TIM4 Channel 2 pulse value
    }

    return 0;
}


void GPIO_config(void)
{
	RCC->IOPENR |= (1U << 0); //Enable clock for GPIOA

	GPIOA->MODER &= ~(3U << 2*0);	// Analog input PA0
	GPIOA->MODER |= (3U << 2*0);
}

// ADC init config.
void ADC_config(void)
{
	RCC->APBENR2 |= (1U << 20); //active ADC Timer
	ADC1->CFGR1 |= (0U << 3);
	ADC1->CR |= (1U << 0);
	ADC1->CHSELR |= (1U << 0);
	ADC1-> SMPR |= (2U << 4);
	ADC->CCR |= (1U << 19);
}


void ADC_read(void)
{
	ADC1->CR |= (1U << 2); //ADC read
	ADC_value = (uint16_t)ADC1->DR; //ADC write value
}


void init_pwm2(void)
{
	RCC->APBENR1 |= 1 << 1; 			// TIM3 enable

	//PA6_TIM3_CH1 ve PA7_TIM3_CH2
	GPIOA->MODER &= ~(3U << 2*6);		// PA6 PWM
	GPIOA->MODER |= (2U << 2*6);

	GPIOA->MODER &= ~(3U << 2*7);		// AF2 PA6
	GPIOA->MODER |= (2U << 2*7);

	GPIOA->AFR[0] &= ~(0xFU << 4*6);	// PA7
	GPIOA->AFR[0] |= (1U << 4*6);

	GPIOA->AFR[0] &= ~(0xFU << 4*7);	// AF2 PA7
	GPIOA->AFR[0] |= (1U << 4*7);

	TIM3->CR1 |= 0 << 4;									// Timer counter mode is Upcounter
	TIM3->CR1 |= 0 << 5;									// Center-aligned mode edge-aligned mode selected
	TIM3->CR1 |= 0 << 8;									// Clock division 1
	TIM3->PSC = 10;											// Prescaler value 10
	TIM3->ARR = 16000;

	TIM3->CCMR1 |= 0 << 0 | 6 << 4 | 0 << 8 | 6 << 12;		// activate output compare mode

	TIM3->CCER |= 1 << 0 | 1 << 4;

	TIM3->CCR1 |= 0;										// TIM4 Channel 1 pulse value
	TIM3->CCR2 |= 0;										// TIM4 Channel 2 pulse value

	TIM3->CR1 |= 1 << 0;									//Enable PWM

}

// Design ADC value
void PWM_Value(){
	if(ADC_value < 60) //low value stat.
	{
		pwm_value = 0;
		pwm_value_2 = 16000;
	}
	else
	{
		pwm_value = k_const*ADC_value; //pwm value
		pwm_value_2 = 16000 - (k_const*ADC_value); //pwm value_2
	}

	if(ADC_value == 4095) // full value stat.
	{
		pwm_value = 16000;
		pwm_value_2 = 0;
	}
}





