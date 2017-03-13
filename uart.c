#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define UART1_DR_R              (*((volatile uint32_t *)0x4000D000))
#define UART1_CTL_R             (*((volatile uint32_t *)0x4000D030))
#define UART1_IBRD_R            (*((volatile uint32_t *)0x4000D024))
#define UART1_FBRD_R            (*((volatile uint32_t *)0x4000D028))
#define UART1_LCRH_R            (*((volatile uint32_t *)0x4000D02C))
#define UART1_CTL_R             (*((volatile uint32_t *)0x4000D030))
#define GPIO_PORTC_AFSEL_R      (*((volatile uint32_t *)0x40006420))
#define GPIO_PORTC_DEN_R        (*((volatile uint32_t *)0x4000651C))
#define GPIO_PORTC_PCTL_R       (*((volatile uint32_t *)0x4000652C))
#define GPIO_PORTC_AMSEL_R      (*((volatile uint32_t *)0x40006528))
#define UART1_FR_R              (*((volatile uint32_t *)0x4000D018))
#define SYSCTL_RCGC1_R          (*((volatile uint32_t *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define UART_FR_RXFE            (*((volatile uint32_t *)0x00000010))  
#define SYSCTL_RCC_R            (*((volatile uint32_t *)0x400FE060))

// Assumes a 80 MHz bus clock, creates 115200 baud rate

void UART_Init(void){            // should be called only once
  SYSCTL_RCC_R  |= 0x024E32D1; //set system clock
  
  SYSCTL_RCGC1_R |= 0x00000002;  // activate UART1

  SYSCTL_RCGC2_R |= 0x00000004;  // activate port C

  UART1_CTL_R &= ~0x00000001;    // disable UART

  UART1_IBRD_R = 43;     // IBRD = int(80,000,000/(16*115,200)) = int(43.40278)

  UART1_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26

  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs

  UART1_CTL_R |= 0x00000001;     // enable UART

  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4

  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1

  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;

  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4

}

// Wait for new input, then return ASCII code

unsigned char UART_InChar(void){

  while((UART1_FR_R&0x0010) != 0);      // wait until RXFE is 0

  return((unsigned char)(UART1_DR_R&0xFF));

}

// Wait for buffer to be not full, then output

void UART_OutChar(unsigned char data){

  while((UART1_FR_R&0x0020) != 0);      // wait until TXFF is 0

  UART1_DR_R = data;

}

// Immediately return input or 0 if no input

unsigned char UART_InCharNonBlocking(void){

  if((UART1_FR_R&UART_FR_RXFE) == 0){

    return((unsigned char)(UART1_DR_R&0xFF));

  } else{

    return 0;

  }

}



 

int main()
{
  UART_Init();
  char c = 'b'; 
  while(1){
  UART_OutChar(c);
   
  }

 return 0;
}
