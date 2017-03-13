#include <stdint.h>

#define STCTRL 0xE000E010
#define STRELOAD 0xE000E014
#define STCURRENT 0xE000E018
#define GPIO_PORTF_AMSEL_R 0x40025528
#define GPIO_PORTF_PCTL_R 0x4002552C
#define GPIO_PORTF_DIR_R 0x40025400
#define GPIO_PORTF_AFSEL_R 0x40025420
#define GPIO_PORTF_DEN_R  0x4002551C
#define GPIO_PORTF_DATA_R 0x400253FC
#define SYSCTL_RCGC2_R 	0x400FE108

int counts;
int count;

//PORTF INITIALISATION
void portf_init(void)
{
volatile uint32_t delay;
*((volatile uint32_t*)SYSCTL_RCGC2_R) |= 0x00000020;	//set clock of PORTF (otherwise port wont work)
delay = SYSCTL_RCGC2_R;					//give time to start up the system clock 
*((volatile uint32_t*)GPIO_PORTF_AMSEL_R) = 0X00;	//disable analog functionality on whole of portf
*((volatile uint32_t*)GPIO_PORTF_PCTL_R) = 0X00;	//to select regular digital func
*((volatile uint32_t*)GPIO_PORTF_DIR_R) = 0X02;		//to set PF1 to o/p mode direction reg
*((volatile uint32_t*)GPIO_PORTF_AFSEL_R) = 0X00;	//clearing to disable alternate functions of port
*((volatile uint32_t*)GPIO_PORTF_DEN_R) = 0x1F;		//to enable digital func
}

//STRELOAD Value calulation
int counter_calc(int ms){
counts = (4 * ms * 1000);
return(counts);
}

// SYSTICK initialisation
void systick_init(count){
*((volatile uint32_t*)STRELOAD) = count;
*((volatile uint32_t*)STCURRENT) = 0x00000000;		//clearing register which also clears COUNT bit of STCTRL register
}

//DELAY func
void delay(){
*((volatile uint32_t*)STCTRL) |= 0x00000001;		// enable counter by setting last bit
while(*((volatile uint32_t*)STCURRENT) != 0x00000000){
}	//wait until counting goes on
*((volatile uint32_t*)STCTRL) &= ~0x00000001;	//disable counter
}

int main(void)
{
portf_init();
count = counter_calc(1000);
systick_init(count);
//Below section blinks the led for exactly 15 times
for(int i = 0; i >= 0; i++){
	*((volatile uint32_t*)GPIO_PORTF_DATA_R) = 0x02;    //make PF1 HIGH
	delay();
	*((volatile uint32_t*)GPIO_PORTF_DATA_R) = 0x00;    //make PF1 LOW
	delay();
}
}
