#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"


#define PWM_FREQUENCY 55

int main(void)
{
    volatile uint32_t ui32Load;
    volatile uint32_t ui32PWMClock;
    int i, j, k;

    //set clock rate
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    //enable PWM1 and GPIOF
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //set PWM clock rate
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    //allow PWM to be edited
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;

    //configure the LED pins
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);
    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //configure the PWM
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);

    //get the period
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

    //set the PWM period
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

    //set the duty cycle of the PWM
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui32Load * 0.9);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui32Load * 0.9);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui32Load * 0.9);

    //enable the PWM
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    //set the PWM to output
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

    while(1)
    {
        //make the red led go from 90% duty cycle to 10%
        for(i = ui32Load * 0.9; i > ui32Load * 0.1; i--)
        {
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, i);
            ROM_SysCtlDelay(10000);
            //make the green led go from 90% duty cycle to 10%
            for(j = ui32Load * 0.9; j > ui32Load * 0.1; j--)
            {
                PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, j);
                ROM_SysCtlDelay(10000);
                //make the blue led go from 90% duty cycle to 10%
                for(k = ui32Load * 0.9; k > ui32Load * 0.1; k--)
                {
                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, k);
                    ROM_SysCtlDelay(10000);
                }
            }
        }
    }
}
