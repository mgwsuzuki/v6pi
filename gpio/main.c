// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : main.c
//
// written by  : mgwsuzuki
//
// date        : Sat May 03 15:09:40 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_gpio.h"

void delay(){
  u32_t n;
  for (n = 0 ; n < 10000000; n++){
    asm("mov r0, r0");
  }
}

int main(){
  u32_t n;
  // ACT LEDのピンをoutputにする
  vp_gpio_set_fsel(16, VP_GPIO_FSEL_OUTPUT);

  while(1){
    vp_gpio_clear_output(16);
    delay();
    vp_gpio_set_output(16);
    delay();
  }
  return 0;
}
