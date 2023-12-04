/*
	Hazirlayan:
 	 Ahmet Ali Tilkicioglu (the1sth)
*/

//SysTick Application

#include "stm32g0xx.h"

void systickInit(void);
void SysTick_Handler(void);
void delay_ms(int ms);

int millis;

int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    systickInit(); // init function for timer

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

    while(1) {
        GPIOC->ODR ^= (1U << 6);
        delay_ms(1000);
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

