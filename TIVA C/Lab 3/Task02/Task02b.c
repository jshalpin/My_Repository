#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


uint8_t ui8PinData=2;       //variable to hold GPIO pin data
int freq;                   //variable for getting the frequency of the board
int i;                      //variable used for the for loops

int main(void)
{
    //set the clock to 8MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_4MHZ|SYSCTL_OSC_MAIN);

    //enables the GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //sets the variables for the GPIO
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //get the clock frequency
    freq = SysCtlClockGet();

    while(1)
    {
        //loop for R, G, B
        for(i=0;i<3;i++)
        {
            //write pin values to the GPIO
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
            //delay for 0.25 seconds
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            SysCtlDelay(2000000);
            //reset the GPIO pin data
            if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
        }
        //set the GPIO pin data to 6
        ui8PinData = 6;
        for(i=0;i<3;i++)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            SysCtlDelay(2000000);
            if(ui8PinData==10) {ui8PinData=12;} else {ui8PinData=ui8PinData+4;}
        }
        //set the GPIO pin data to 14
        ui8PinData = 14;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
        SysCtlDelay(2000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
        SysCtlDelay(2000000);
        ui8PinData = 2;
    }

}
