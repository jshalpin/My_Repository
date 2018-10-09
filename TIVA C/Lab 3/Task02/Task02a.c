#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


uint8_t ui8PinData=2;       //variable to hold GPIO pin data
int freq;                   //variable for getting the frequency of the board

int main(void)
{
    //set the clock to 800KHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_4MHZ|SYSCTL_OSC_MAIN);

    //enables the GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //sets the variables for the GPIO
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1);

    //get the clock frequency
    freq = SysCtlClockGet();

    while(1)
    {
        //write pin values to the GPIO
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
        //delay 0.425 seconds
        SysCtlDelay(340000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
        SysCtlDelay(340000);
        //reset the GPIO pin data
        if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
    }

}