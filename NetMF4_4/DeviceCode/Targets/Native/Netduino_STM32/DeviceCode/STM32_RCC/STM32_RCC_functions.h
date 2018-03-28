#ifndef STM32_RCC_FUNCTIONS_H
#define STM32_RCC_FUNCTIONS_H

#define RCC_AHB1Peripheral_GPIOA             ((UINT32)0x00000001)
#define RCC_AHB1Peripheral_GPIOB             ((UINT32)0x00000002)
#define RCC_AHB1Peripheral_GPIOC             ((UINT32)0x00000004)
#define RCC_AHB1Peripheral_GPIOD             ((UINT32)0x00000008)
#define RCC_AHB1Peripheral_GPIOE             ((UINT32)0x00000010)
#define RCC_AHB1Peripheral_GPIOF             ((UINT32)0x00000020)
#define RCC_AHB1Peripheral_GPIOG             ((UINT32)0x00000040)
#define RCC_AHB1Peripheral_GPIOH             ((UINT32)0x00000080)
#define RCC_AHB1Peripheral_GPIOI             ((UINT32)0x00000100)

#define RCC_AHB2Peripheral_OTG_FS            ((UINT32)0x00000080)

#define RCC_APB1Peripheral_PWR               ((UINT32)0x10000000)

#define RCC_APB2Peripheral_SYSCFG            ((UINT32)0x00004000)

void STM32_RCC_AHB1PeripheralClockEnable(UINT32 peripheral);
void STM32_RCC_AHB1PeripheralClockDisable(UINT32 peripheral);

void STM32_RCC_AHB2PeripheralClockEnable(UINT32 peripheral);
void STM32_RCC_AHB2PeripheralClockDisable(UINT32 peripheral);

void STM32_RCC_APB1PeripheralClockEnable(UINT32 peripheral);
void STM32_RCC_APB1PeripheralClockDisable(UINT32 peripheral);

void STM32_RCC_APB2PeripheralClockEnable(UINT32 peripheral);
void STM32_RCC_APB2PeripheralClockDisable(UINT32 peripheral);

void STM32_RCC_APB1PeripheralResetEnable(UINT32 peripheral);
void STM32_RCC_APB1PeripheralResetDisable(UINT32 peripheral);

#endif
