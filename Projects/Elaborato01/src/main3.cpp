#include "mbed.h"

int main3() {
    AnalogIn input(A0);
    PwmOut led(LED1);

    float value = 0;
    led.period(0.001);

    while(1){
        
        value = input.read();
        led.write(value);

    }


}
