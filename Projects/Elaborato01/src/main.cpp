#include "mbed.h"

AnalogIn input(A0);

int main() {

  // put your setup code here, to run once:
  uint16_t samples[1024];

  for(int i=0; i<1024;i++){
    samples[i] = input.read_u16();
    wait(0.001f);
  }

  printf("Result: \n");
  for(int i=0; i<1024; i++){
    printf("%d, 0x%04X\n",i,samples[i]);
  }

  while(1) {
    // put your main code here, to run repeatedly:
    
  }
}
