/*
	Hazirlayan:
 	 Ahmet Ali Tilkicioglu (the1sth)
*/

// LED delay settings with EXTI Application

#include "stm32g0xx.h"

void systickInit(void);
void SysTick_Handler(void);
void delay_ms(int ms);

int millis;
int delay = 1000;
int flag = 1;


int main(void) {

    /* Enable GPIOA-GPIOB-GPIOC clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Setup PB8 as input */
    GPIOB->MODER &= ~(3U << 2*8);


    systickInit(); // init function for timer

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

    EXTI_config();

    while(1) {


        GPIOC->ODR ^= (1U << 6);
        delay_ms(delay*flag);
    }

    return 0;
}

void SysTick_Handler(void) // kesme oluştuğunda girilecek fonksiyon
{
	millis++;      // millis değerini 1 arttır
}

void delay_ms(int ms)  // milisaniye cinsinden delay fonksiyonu
{
	int delay = millis+ms;
	while(millis<delay);
}

void systickInit(void)
{

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Sayıcıyı aktif et
	SysTick->LOAD = 1999;                     // Reload değerini ata
	SysTick->VAL=0;                        // sayıcıyı sıfırla
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  // kesmeyi aktif et
	SysTick->CTRL &= ~(1<<2);                      //Systick clock source = AHB/8
	NVIC_EnableIRQ(SysTick_IRQn);     // NVIC tarafında kesmeyi aç
	NVIC_SetPriority (SysTick_IRQn,0);  //kesme önceliğini ayarla

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

	flag = flag + 1;
	if(flag == 11)
		flag = 1;

	EXTI->RPR1 |= (1U << 8);
}
