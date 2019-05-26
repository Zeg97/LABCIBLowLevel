#include <mbed.h>

#define TIM_USR TIM2
#define TIM_USR_IRQ TIM2_IRQn
#define PRESCALER 83
#define PERIOD 9999

volatile char flag_time = 0;

TIM_HandleTypeDef mTimUserHandle;

extern "C" void M_TIM_USR_Handler(void) {
  if (__HAL_TIM_GET_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE) == SET) {
    __HAL_TIM_CLEAR_FLAG(&mTimUserHandle, TIM_FLAG_UPDATE);
    flag_time++;
  }
}

int main() {

  // put your setup code here, to run once:

  AnalogIn pot(PB_1);

  DigitalOut one_e(PC_10);
  PwmOut one_v(PA_8);
  DigitalOut two_e(PC_11);
  PwmOut two_v(PA_9);
  DigitalOut thr_e(PC_12);
  PwmOut thr_v(PA_10);
  DigitalOut enable_chip(PA_6);

  

  enable_chip.write(1);

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

  float vel = 0.5f;
  one_v.period(0.0001f);
  two_v.period(0.0001f);
  thr_v.period(0.0001f);

  while (1) {
     
    switch (flag_time) {
      case 1:
        one_e.write(0);
        two_e.write(1);
        thr_e.write(1);
        one_v.write(0);
        two_v.write(vel);
        thr_v.write(0);
        break;
      case 2:
        
        one_e.write(1);
        two_e.write(1);
        thr_e.write(0);
        one_v.write(0);
        two_v.write(vel);
        thr_v.write(0);
        break;
      case 3:
        
        one_e.write(1);
        two_e.write(0);
        thr_e.write(1);
        one_v.write(0);
        two_v.write(0);
        thr_v.write(vel);
        break;
      case 4:
        
        one_e.write(0);
        two_e.write(1);
        thr_e.write(1);
        one_v.write(0);
        two_v.write(0);
        thr_v.write(vel);
        break;
      case 5:
        
        one_e.write(1);
        two_e.write(1);
        thr_e.write(0);
        one_v.write(vel);
        two_v.write(0);
        thr_v.write(0);
        break;
      case 6:
        
        one_e.write(1);
        two_e.write(0);
        thr_e.write(1);
        one_v.write(vel);
        two_v.write(0);
        thr_v.write(0);
        break;
      default:
        flag_time=1;
      
        break;
    }
  }
}