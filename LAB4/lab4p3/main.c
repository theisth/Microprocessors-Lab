/*
 * Hazirlayanlar:
 * 		Ahmet Ali Tilkicioglu
 * 		Burak Balkan
 * 		İsmail Ağca
 */

#include "stm32g0xx.h"

void printChar(uint8_t c);
void __print(char *ptr, int len);
void uart_tx(uint8_t c);
uint8_t uart_rx(void);

void GPIO_Config(void);
void init_pwm2(void);
void PWM_Value();

uint16_t pwm_value = 0;
int counter = 0;
int factor = 0;

char key_value_1 = '0';
char key_value_2 = '0';
uint8_t password = 0;
void read_Key(void);


int main(void) {

	GPIO_Config();
	USART_Config(9600);
	init_pwm2();

	while(1) {

    }

    return 0;
}

void GPIO_Config(void){
	RCC->IOPENR |= (1U << 0); //Enable clock for GPIOA
	RCC->IOPENR |= (1U << 1); //Enable clock for GPIOB
	RCC->APBENR1 |= (1U << 17); //Enable clock for USART2

	GPIOA->MODER &= ~(3U << 2*2); //PA2 USART
	GPIOA->MODER |= (2U << 2*2);

	GPIOA->AFR[0] &= ~(0xFU << 4*2);
	GPIOA->AFR[0] |= (1 << 4*2);

	GPIOA->MODER &= ~(0xFU << 2*3); //PA3 USART
	GPIOA->MODER |= (2U << 2*3);

	GPIOA->AFR[0] &= ~(0xFU << 4*3);
	GPIOA->AFR[0] |= (1 << 4*3);

	GPIOB->MODER &= ~(3U << 2*1); // PB1 output
	GPIOB->MODER |= (1U << 2*1);

	GPIOB->MODER &= ~(3U << 2*2); // PB2 output
	GPIOB->MODER |= (1U << 2*2);

	GPIOB->MODER &= ~(3U << 2*3); // PB3 output
	GPIOB->MODER |= (1U << 2*3);

	GPIOB->MODER &= ~(3U << 2*4); // PB4 output
	GPIOB->MODER |= (1U << 2*4);

    GPIOA->MODER &= ~(3U << 2*4); // PA4 input
    GPIOA->MODER &= ~(3U << 2*5); // PA5 input
    GPIOA->MODER &= ~(3U << 2*6); // PA6 input
    GPIOA->MODER &= ~(3U << 2*7); // PA7 input

    GPIOA->PUPDR &= ~(3U << 2*4); //reset bits
    GPIOA->PUPDR &= ~(3U << 2*5);
	GPIOA->PUPDR &= ~(3U << 2*6);
	GPIOA->PUPDR &= ~(3U << 2*7);

    GPIOA->PUPDR |= (1U << 2*4); //Pull Up Resistor Active
    GPIOA->PUPDR |= (1U << 2*5);
    GPIOA->PUPDR |= (1U << 2*6);
    GPIOA->PUPDR |= (1U << 2*7);

}


void __print(char *ptr, int len){
	for(int i=0; i<len ; ++i){
			printChar(ptr[i]);
		}
}

void printChar(uint8_t c){
	USART2->TDR = (uint16_t) c;
	while(!(USART2->ISR & (1 << 6))); // 6.bit transmission complete
}


void uart_tx(uint8_t c){
	USART2->TDR = (uint16_t) c;
	while(!(USART2->ISR & (1 << 6))); // 6.bit transmission complete
}


uint8_t uart_rx(void){
	while (!(USART2->ISR & (1 << 5))); //RXFNE bit of ISR, receive data ready to read

	return (uint8_t)USART2->RDR;
	//RXNE is automatically cleared when read
}


void USART_Config(uint16_t baud){
	USART2->CR1 = 0;
	USART2->CR1 |= (1U << 2); //USART1 receiver enable
	USART2->CR1 |= (1U << 3); //USART1 transmitter enable
	//USART2->CR1 |= (1U << 5); //RX Interrupt enable

	USART2->BRR  = (uint16_t)(SystemCoreClock / baud); //Setting baudrate

	USART2->CR1 |= (1U << 0); //USART2 enable

	//NVIC_SetPriority(USART2_IRQn , 1);
	//NVIC_EnableIRQ(USART2_IRQn);

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
	GPIOA->AFR[0] |= (2U << 4*1);
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
	pwm_value = 1600*factor; //pwm value
}

void TIM2_IRQHandler()
{
	read_Key();
	PWM_Value();
	TIM2-> CCR2 = pwm_value;

	counter = counter + 1;
	if(counter == 200000) //read duty cycle value
	{
		printChar(key_value_1);
		printChar(key_value_2);
		printChar('\n');
		printChar('\r');
		counter = 0;
	}
}

void read_Key(void)
{
	GPIOB->ODR |= (0x1E << 0); //all high
	GPIOB->ODR &= ~(1U << 1); //PB1 low

	if(!(GPIOA->IDR & (1U << 4)))
	{
		password = 1; //1

	}
	if(!(GPIOA->IDR & (1U << 5)))
	{
		password = 0; //2
	}
	if(!(GPIOA->IDR & (1U << 6)))
	{
		password = 0; //3
	}
	if(!(GPIOA->IDR & (1U << 7)))
	{
		password = 0; //A
	}


	GPIOB->ODR |= (0x1E << 0);
	GPIOB->ODR &= ~(1U << 2); //PB2 low

	if(!(GPIOA->IDR & (1U << 4)))
	{
		password = 0; //4
	}
	if(!(GPIOA->IDR & (1U << 5)))
	{
		password = 0; //5
	}
	if(!(GPIOA->IDR & (1U << 6)))
	{
		password = 0; //6
	}
	if(!(GPIOA->IDR & (1U << 7)))
	{
		password = 0; //B
	}

	GPIOB->ODR |= (0x1E << 0);
	GPIOB->ODR &= ~(1U << 3); //PB3 low

	if(!(GPIOA->IDR & (1U << 4)))
	{
		password = 0; //7
	}
	if(!(GPIOA->IDR & (1U << 5)))
	{
		password = 0; //8
	}
	if(!(GPIOA->IDR & (1U << 6)))
	{
		password = 9; //9
	}
	if(!(GPIOA->IDR & (1U << 7)))
	{
		password = 0; //C
	}


	GPIOB->ODR |= (0x1E << 0);
	GPIOB->ODR &= ~(1U << 4); //PB4 low

	if(!(GPIOA->IDR & (1U << 4)))
	{
		password = 0; //*
	}
	if(!(GPIOA->IDR & (1U << 5)))
	{
		if(password == 1) //0
			password = 2;
		if(password == 9)
			password =10;
	}
	if(!(GPIOA->IDR & (1U << 6)))
	{
		if(password == 10) //#
			password = 11;
		if(password == 2)
			password = 3;
	}
	if(!(GPIOA->IDR & (1U << 7)))
	{
		password = 0; //D
	}

	if(password == 3) //10# password
	{
		factor = 1;
		key_value_1 = '1';
		key_value_2 = '0';
		printChar(key_value_1);
		printChar(key_value_2);
		printChar('\n');
		printChar('\r');
		counter = 0;
		password = 0;
	}
	if(password == 11) //90# password
	{
		factor = 9;
		key_value_1 = '9';
		key_value_2 = '0';
		printChar(key_value_1);
		printChar(key_value_2);
		printChar('\n');
		printChar('\r');
		counter = 0;
		password = 0;
	}
}
