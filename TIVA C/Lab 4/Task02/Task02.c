#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_gpio.h"
int main(void)
{
    //set the clock frequency to 16MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //enable GPIO F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //enable GPIO E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    HWREG(GPIO_PORTE_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTE_BASE + GPIO_O_CR) |= GPIO_PIN_0;

    //set the output pins for the LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //enables the pins connected to the switch as inputs
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
    //enables a specific event within the GPIO to generate an interrupt
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_0);/////
    //sets interrupt to rising edge on GPIO
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_INT_PIN_0, GPIO_RISING_EDGE);////

    IntEnable(INT_GPIOE);////;
    //IntMasterEnable();

    //loop forever
    while(1)
    {
    }
}

void PortEPin0IntHandler(void)
{
    // Clear the GPIO interrupt
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_INT_PIN_0);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
    timer1A_delaySec(1);
}

void timer1A_delaySec(int ttime){
    int i;

    SYSCTL_RCGCTIMER_R |= 2;        //enable clock to timer block 1
    TIMER1_CTL_R = 0;               //disable timer before initialization
    TIMER1_CFG_R = 0x04;            //16-bit option
    TIMER1_TAMR_R = 0x02;           //periodic mode and down-counter
    TIMER1_TAILR_R = 64000 - 1;     //timera interval load vlaue reg
    TIMER1_TAPR_R = 250 - 1;        //timera prescaler
    TIMER1_ICR_R = 0x1;             //clear the timera timeout flag
    TIMER1_CTL_R |= 0x01;           //enable timer a after initialization
    for(i=0; i<ttime; i++)
    {
        while((TIMER1_RIS_R & 0x1) == 0);
        TIMER1_ICR_R = 0x1;         //clear the timera timeout flag
    }
}
