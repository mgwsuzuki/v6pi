// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_gpio.c
//
// written by  : (ken)
//
// date        : Sat May 03 14:40:10 2014 +0900
//
////////////////////////////////////////////////////////////

#include "vp_gpio.h"

static inline u32_t bset(i32_t pos){
  return (1 << pos);
}

// 指定された番号のGPIOピンのfunctionをセットする
void vp_gpio_set_fsel(i32_t portnum, vp_gpio_fsel_t fsel){

  volatile u32_t* regp;
  u32_t pos;
  u32_t v;

  if (portnum < 10){
    regp = (volatile u32_t*)GPFSEL0;
    pos = portnum * 3;

  }else if (portnum < 20){
    regp = (volatile u32_t*)GPFSEL1;
    pos = (portnum-10) * 3;

  }else if (portnum < 30){
    regp = (volatile u32_t*)GPFSEL2;
    pos = (portnum-20) * 3;

  }else if (portnum < 40){
    regp = (volatile u32_t*)GPFSEL3;
    pos = (portnum-30) * 3;

  }else if (portnum < 50){
    regp = (volatile u32_t*)GPFSEL4;
    pos = (portnum-40) * 3;

  }else if (portnum < 54){
    regp = (volatile u32_t*)GPFSEL5;
    pos = (portnum-50) * 3;
  }else{
    return;
  }

  v = *regp & ~(7 << pos);
  *regp = v | (fsel << pos);
}

// 指定された番号のGPIOピンの出力を1にする
void vp_gpio_set_output(i32_t portnum){

  volatile u32_t* regp;
  u32_t v;
  if (portnum < 32){
    regp = (volatile u32_t*)GPSET0;
    v = bset(portnum);
  }else if (portnum < 54){
    regp = (volatile u32_t*)GPSET1;
    v = bset(portnum-32);
  }else{
    return;
  }
  *regp = v;
}

// 指定された番号のGPIOピンの出力を0にする
void vp_gpio_clear_output(i32_t portnum){
  volatile u32_t* regp;
  u32_t v;
  if (portnum < 32){
    regp = (volatile u32_t*)GPCLR0;
    v = bset(portnum);
  }else if (portnum < 54){
    regp = (volatile u32_t*)GPCLR1;
    v = bset(portnum-32);
  }else{
    return;
  }
  *regp = v;
}

// 指定された番号のGPIOピンのpull-up/downをセットする
void vp_gpio_set_pud(i32_t portnum, vp_gpio_pud_t pud){

  volatile u32_t* pudp = (volatile u32_t*)GPPUD;
  volatile u32_t* pudclkp;
  u32_t n;
  u32_t v;

  if (portnum < 32){
    pudclkp = (volatile u32_t*)GPPUDCLK0;
    v = bset(portnum);
  }else if (portnum < 54){
    pudclkp = (volatile u32_t*)GPPUDCLK1;
    v = bset(portnum - 32);
  }else{
    return;
  }

  *pudp = pud;
  for (n = 0; n < 150; n++){
    asm("mov r0,r0");
  }
  *pudclkp = v;
  for (n = 0; n < 150; n++){
    asm("mov r0,r0");
  }
  *pudclkp = 0;
}

// 指定された番号のGPIOピンのレベルをセットする
void vp_gpio_set_level(i32_t portnum, u32_t level){

  if (level == 0){
    vp_gpio_clear_output(portnum);
  }else{
    vp_gpio_set_output(portnum);
  }
}

// 指定された番号のGPIOピンのレベルを返す
u32_t vp_gpio_get_level(i32_t portnum){

  volatile u32_t* regp;
  u32_t pos;
  u32_t v;

  if (portnum < 32){
    regp = (volatile u32_t*)GPLEV0;
    pos = portnum;
  }else if (portnum < 54){
    regp = (volatile u32_t*)GPLEV1;
    pos = portnum - 32;
  }else{
    return 0;
  }

  v = *regp;
  return (v >> pos) & 1;
}
