#include "mbed.h"
#define TIM_USR TIM2
#define TIM_USR_IRQ TIM2_IRQn

/*
    1Hz : PRE 9999, PER 9999
    100Hz : PRE 999, PER 999
    1KHz : PRE 99, PER 999
    10KHz : PRE 99, PER 99
*/

#define PRESCALER 99
#define PERIOD 99

volatile char flag_time = 0;

TIM_HandleTypeDef mTimUserHandle;



extern "C"
void M_TIM_USR_Handler(void) {
    if(__HAL_TIM_GET_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE) == SET) {
        __HAL_TIM_CLEAR_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE);
        flag_time = 1;
    }
}

int main() {

    AnalogIn input(A0);

    float measure;

    __HAL_RCC_TIM2_CLK_ENABLE();

    mTimUserHandle.Instance = TIM_USR;
    mTimUserHandle.Init.Prescaler = PRESCALER;
    mTimUserHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    mTimUserHandle.Init.Period = PERIOD;
    mTimUserHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&mTimUserHandle);
    HAL_TIM_Base_Start_IT(&mTimUserHandle);

    NVIC_SetVector(TIM_USR_IRQ, (uint32_t)M_TIM_USR_Handler);
    NVIC_EnableIRQ(TIM_USR_IRQ);

    while(1) {
        if(flag_time == 1) {
            flag_time = 0;
            measure = input.read();
            printf("%f\n",measure);
        } 
    }
}