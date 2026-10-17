#include "stm32f10x.h"

void GPIO_Config(void){
    // Relojes: GPIOA, GPIOC y AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    // PC13: salida push-pull, 2 MHz
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |=  (0x2 << 20);
    GPIOC->BSRR = (1 << 13);        // LED apagado al inicio (activo en bajo)

    // PA0: entrada con pull-up/pull-down, y ODR=0 lo deja en pull-down
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |=  (0x8 << 0);
    GPIOA->BRR = (1 << 0);
}

void EXTI0_Config(void){
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0;   // EXTI0 conectado a PA0
    EXTI->IMR  |= EXTI_IMR_MR0;               // habilita la línea 0
    EXTI->RTSR |= EXTI_RTSR_TR0;              // flanco de subida
    EXTI->FTSR &= ~EXTI_FTSR_TR0;             // sin flanco de bajada
    NVIC_EnableIRQ(EXTI0_IRQn);               // habilita en el NVIC
}

void EXTI0_IRQHandler(void){
    if (EXTI->PR & EXTI_PR_PR0){
        EXTI->PR = EXTI_PR_PR0;     // limpiar bandera (se escribe 1)
        GPIOC->BRR = (1 << 13);     // PC13 = 0 -> enciende el LED
    }
}

int main(void){
    GPIO_Config();
    EXTI0_Config();

    while (1){
	}
}