#include "mbed.h"
#include "DigitalEncoderAB.h"
#include "DigitalEncoderAS5601.h"
#include "DigitalEncoderPIC.h"

float step1 = 360/42.0;
float step2 = step1*2;
float step3 = step1*3;
float step4 = step1*4;
float step5 = step1*5;
float step6 = step1*6;


int stepChange(float angle_sect){

  if((angle_sect > 0.01f) && (angle_sect <= step1)){
      return 5;
    } else if (angle_sect > step1 && angle_sect <= step2){
      return 6;
    } else if (angle_sect > step2 && angle_sect <= step3){
      return 1;
    } else if (angle_sect > step3 && angle_sect <= step4){
      return 2;
    } else if (angle_sect > step4 && angle_sect <= step5){
      return 3;
    } else if (angle_sect > step5) {
      return 4;
    }
}

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
  
  float angle_section = 360/7.0;

  enable_chip.write(1);

  float vel = 0.8f;
  one_v.period(0.00001f);
  two_v.period(0.00001f);
  thr_v.period(0.00001f);

  asm(".global _printf_float");

  float angle_deg, angle_rad;

  //encoder.setAngleFormat(angleContinuous);

  int count = 0;
  float i = 0;

  while(1) {

    // read the angle
    angle_deg = encoder.getAngleDeg();
    float offset = pot.read()*360/7;
    //offset = (float)((int)(offset * 100 + .5))/100;
    float angle_sect=fmod(angle_deg+offset, angle_section) ;
   // pc.printf("%f\n", angle_sect);
    int flag_time = stepChange(angle_sect);
    //pc.printf("%i\n",flag_time);
   
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

    }
    wait(0.0001);
  }
}

