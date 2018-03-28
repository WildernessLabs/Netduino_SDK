#include <tinyhal.h>
#include "..\stm32.h"
#include "STM32_RCC_functions.h"

void STM32_RCC_AHB1PeripheralClockEnable(UINT32 peripheral)
{
    RCC->AHB1ENR |= peripheral;
}

void STM32_RCC_AHB1PeripheralClockDisable(UINT32 peripheral)
{
    RCC->AHB1ENR &= ~peripheral;
}

void STM32_RCC_AHB2PeripheralClockEnable(UINT32 peripheral)
{
    RCC->AHB2ENR |= peripheral;
}

void STM32_RCC_AHB2PeripheralClockDisable(UINT32 peripheral)
{
    RCC->AHB2ENR &= ~peripheral;
}

void STM32_RCC_APB1PeripheralClockEnable(UINT32 peripheral)
{
    RCC->APB1ENR |= peripheral;
}

void STM32_RCC_APB1PeripheralClockDisable(UINT32 peripheral)
{
    RCC->APB1ENR &= ~peripheral;
}

void STM32_RCC_APB2PeripheralClockEnable(UINT32 peripheral)
{
    RCC->APB2ENR |= peripheral;
}

void STM32_RCC_APB2PeripheralClockDisable(UINT32 peripheral)
{
    RCC->APB2ENR &= ~peripheral;
}

void STM32_RCC_APB1PeripheralResetEnable(UINT32 peripheral)
{
    RCC->APB1RSTR |= peripheral;
}

void STM32_RCC_APB1PeripheralResetDisable(UINT32 peripheral)
{
    RCC->APB1RSTR &= ~peripheral;
}