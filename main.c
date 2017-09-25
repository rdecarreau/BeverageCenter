#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "sysctl.h"
#include "interrupt.h"
#include "gpio.h"
#include "timer.h"
int main(void)
{
	unsigned long ulPeriod;
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_FALLING_EDGE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
	ulPeriod = (SysCtlClockGet() / 2) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod -1);


	IntEnable(INT_TIMER0A);
	IntEnable(INT_GPIOF);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);

	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);
	while(1)
	{
	}
}
void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
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
}
void GPIOFIntHandler(void)
{
	// Clear the GPIO interrupt
	GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	// Disable the timer
	TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

