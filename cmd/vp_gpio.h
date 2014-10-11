// -*- coding:utf-8 -*-
////////////////////////////////////////////////////////////
//
// filename    : vp_gpio.h
//
// written by  : mgwsuzuki
//
// date        : Sat May 03 12:46:22 2014 +0900
//
////////////////////////////////////////////////////////////

#ifndef VP_GPIO_H
#define VP_GPIO_H

#include "vp_typedef.h"

// レジスタアドレス
// function select
#define GPFSEL0   0x20200000
#define GPFSEL1   0x20200004
#define GPFSEL2   0x20200008
#define GPFSEL3   0x2020000c
#define GPFSEL4   0x20200010
#define GPFSEL5   0x20200014
// output set
#define GPSET0    0x2020001c
#define GPSET1    0x20200020
// output clear
#define GPCLR0    0x20200028
#define GPCLR1    0x2020002c
// level
#define GPLEV0    0x20200034
#define GPLEV1    0x20200038
// event detect status
#define GPEDS0    0x20200040
#define GPEDS1    0x20200044
// rising edge detect enable
#define GPREN0    0x2020004c
#define GPREN1    0x20200050
// falling edge detect enable
#define GPFEN0    0x20200058
#define GPFEN1    0x2020005c
// high detect enable
#define GPHEN0    0x20200064
#define GPHEN1    0x20200068
// low detect enable
#define GPLEN0    0x20200070
#define GPLEN1    0x20200074
// async. rising edge detect
#define GPAREN0   0x2020007c
#define GPAREN1   0x20200080
// async. falling edge detect
#define GPAFEN0   0x20200088
#define GPAFEN1   0x2020008c
// pull-up/down enable, clock
#define GPPUD     0x20200094
#define GPPUDCLK0 0x20200098
#define GPPUDCLK1 0x2020009c

// function select
typedef enum {
  VP_GPIO_FSEL_INPUT  = 0,
  VP_GPIO_FSEL_OUTPUT = 1,
  VP_GPIO_FSEL_ALT0   = 4,
  VP_GPIO_FSEL_ALT1   = 5,
  VP_GPIO_FSEL_ALT2   = 6,
  VP_GPIO_FSEL_ALT3   = 7,
  VP_GPIO_FSEL_ALT4   = 3,
  VP_GPIO_FSEL_ALT5   = 2
} vp_gpio_fsel_t;

// pull-up/down
typedef enum {
  VP_GPIO_PUD_OFF  = 0,
  VP_GPIO_PUD_DOWN = 1,
  VP_GPIO_PUD_UP   = 2
} vp_gpio_pud_t;

////////////////////////////////////////
//

// 指定された番号のGPIOピンのfunctionをセットする
void vp_gpio_set_fsel(i32_t portnum, vp_gpio_fsel_t fsel);

// 指定された番号のGPIOピンの出力を1にする
void vp_gpio_set_output(i32_t portnum);

// 指定された番号のGPIOピンの出力を0にする
void vp_gpio_clear_output(i32_t portnum);

// 指定された番号のGPIOピンのpull-up/downをセットする
void vp_gpio_set_pud(i32_t portnum, vp_gpio_pud_t pud);

// 指定された番号のGPIOピンのレベルをセットする
void vp_gpio_set_level(i32_t portnum, u32_t level);

// 指定された番号のGPIOピンのレベルを返す
u32_t vp_gpio_get_level(i32_t portnum);

#endif	// !VP_GPIO_H
