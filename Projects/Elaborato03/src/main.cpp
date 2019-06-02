#include <mbed.h>
//#include "DigitalEncoderAB.h"
#include "DigitalEncoderAS5601.h"
//#include "DigitalEncoderPIC.h"

int main() {

  //creating an object of serial class to communicate with PC
  Serial pc(SERIAL_TX, SERIAL_RX);

  // AS5601 encoder
  DigitalEncoderAS5601 encoder(PB_9, PB_8);

  // put your setup code here, to run once:
  AnalogIn pot(PB_1);

  DigitalOut one_e(PC_10);
  PwmOut one_v(PA_8);
  DigitalOut two_e(PC_11);
  PwmOut two_v(PA_9);
  DigitalOut thr_e(PC_12);
  PwmOut thr_v(PA_10);
  DigitalOut enable_chip(PA_6);
  
  volatile char flag_time = 0;
  float step = 360/42.0;

  enable_chip.write(1);

  float vel = 0.5f;
  one_v.period(0.0001f);
  two_v.period(0.0001f);
  thr_v.period(0.0001f);

  asm(".global _printf_float");

  float angle_deg, angle_rad;

  //encoder.setAngleFormat(angleContinuous);

  int count = 0;
  float i = 0;

  while(1) {

    // read the angle
    angle_deg = encoder.getAngleDeg();
    angle_rad = encoder.getAngleRad();
    i=angle_deg - (((int)(angle_deg / 51.428f)) * 51.428f) ;
    // pc.printf("%f, %f, %f\n",angle_deg,angle_rad, i);
   
    if(0<= i && i <step){
      one_e.write(0);
      two_e.write(1);
      thr_e.write(1);
      one_v.write(0);
      two_v.write(vel);
      thr_v.write(0);
    } else if(step<= i && i <step*2){
      one_e.write(1);
      two_e.write(1);
      thr_e.write(0);
      one_v.write(0);
      two_v.write(vel);
      thr_v.write(0);
    } else if(step*2<= i && i <step*3){
      one_e.write(1);
      two_e.write(0);
      thr_e.write(1);
      one_v.write(0);
      two_v.write(0);
      thr_v.write(vel);
    } else if(step*3<= i && i <step*4){
      one_e.write(0);
      two_e.write(1);
      thr_e.write(1);
      one_v.write(0);
      two_v.write(0);
      thr_v.write(vel);
    }else if(step*4<= i && i <step*5){
      one_e.write(1);
      two_e.write(1);
      thr_e.write(0);
      one_v.write(vel);
      two_v.write(0);
      thr_v.write(0);
    }else if(step*5<= i && i <step*6){
      one_e.write(1);
      two_e.write(0);
      thr_e.write(1);
      one_v.write(vel);
      two_v.write(0);
      thr_v.write(0);
    }

    wait(0.001);
        
  }
}