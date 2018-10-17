#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "driverlib/Nokia5110.h"

uint32_t ui32ADC0Value[4];
volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;
char temperature[2];

int main(void)
{
	SysTick_Init();
	startSSI0();
	initialize_screen(BACKLIGHT_ON,SSI0);
	int i;
	int max=11,current_pos=0;
	set_buttons_up_down();
	unsigned char menu_elements[12][25];
	menu_elements[0][0]='1';
	menu_elements[0][1]=0x00;
	menu_elements[1][0]='2';
	menu_elements[1][1]=0x00;
	menu_elements[2][0]='3';
	menu_elements[2][1]=0x00;
	menu_elements[3][0]='4';
	menu_elements[3][1]=0x00;
	menu_elements[4][0]='5';
	menu_elements[4][1]=0x00;
	menu_elements[5][0]='6';
	menu_elements[5][1]=0x00;
	menu_elements[6][0]='7';
	menu_elements[6][1]=0x00;
	menu_elements[7][0]='8';
	menu_elements[7][1]=0x00;
	menu_elements[8][0]='9';
	menu_elements[8][1]=0x00;
	menu_elements[9][0]='1';
	menu_elements[9][1]='0';
	menu_elements[9][2]=0x00;
	menu_elements[10][0]='1';
	menu_elements[10][1]='1';
	menu_elements[10][2]=0x00;
	menu_elements[11][0]='1';
	menu_elements[11][1]='2';
	menu_elements[11][2]=0x00;
	set_menu(menu_elements);

	//enable the ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    //set the amount for averaging
    ADCHardwareOversampleConfigure(ADC0_BASE, 32);

    //select the proper ADC and fifo
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);

	while(1)
	{
		getTemp();
	}
	return 0;
}

void getTemp(void)
{
	char str[6];
	char temp[3];
    //clear the interrupt
    ADCIntClear(ADC0_BASE, 1);
    ADCProcessorTrigger(ADC0_BASE, 1);

    //wait for the interrupt flag
    while(!ADCIntStatus(ADC0_BASE, 1, false))
    {
    }

    //get the data from the buss
    ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
    //average data
    ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
    //convert to celcius
    ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
    //convert to fahrenheit
    ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

    tostring(temp, ui32TempValueF);

    str[0] = 'F';
    str[1] = ' ';
    str[2] = '=';
    str[3] = ' ';
    str[4] = temp[0];
    str[5] = temp[1];
    str[6] = temp[2];
	clear_screen(SSI0);
	screen_write(str, ALIGN_CENTRE_CENTRE,SSI0);
	SysTick_Wait50ms(100);
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;

    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

// The delay parameter is in units of the 16 MHz core clock.
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}


void SysTick_Wait50ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 50ms
  }
}


void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
