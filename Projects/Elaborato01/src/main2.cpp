#include "mbed.h"

AnalogIn input(A0);

int main2() {
  
  uint16_t measure;
  int i = 0;

  while(1){
    measure = input.read_u16();
    printf("%d, 0x%04X\n",i,measure);
    i++;
    wait(0.001f);
  }


}
